#include <queue>
#include <array>
#include <cstdint>

using Entity = std::uint_32t;
const Entity MAX_ENTITIES = 5000;

struct Transform{
	uint positionSize;
	uint rotationSize;
	uint scaleSize;
	float * position;
	float * rotation;
	float * scale;
}

using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager{
public:
	EntityManager(){
		// Initialize the queue with all possible entity IDs
		for(Entity entity = 0; entity < MAX_ENTITIES; ++entity){
			m_availableEntities.push(entity);
		}
	}

	Entity createEntity(){
		assert(m_livingEntityCount < MAX_ENTITIES && "Too many entities in existance");

		// Take an ID from the front of the queue
		Entity id = m_availableEntities.front();
		m_availableEntities.pop();
		++ m_livingEntityCount;

		return id;
	}

	void destroyEntity(Entity entity){
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		// Invalidate the detroyed entity's signature
		m_signatures[entity].reset();

		// Put the destroyed ID at the back of the queue
		m_availableEntities.push(entity);
		--m_livingEntityCount;
	}

	void setSignature(Entity entity, Signature signature){
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		// Put this entity's signature into the array
		m_signatures[entity] = signature;
	}

	Siganture getSignature(Entity entity){
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		// Get this entity's signature from the array
		return m_signatures[entity];
	}
private:
	// Queue of unused entity IDs
	std::queue<Entity> m_availableEntities;

	// Array of signatures where the index corresponds to the entity ID
	std::array<Signature, MAX_ENTITIES> m_signatures{};

	// Total living entities - used to keep limits on how many exist
	std::uint32_t m_livingEntityCount{};
};

// The one instance of virtual inheritance in the entire implementation.
// An interface is needed so that the ComponentManager (seen later)
// can tell a generic ComponentArray that an entity has been destroyed
// and that it needs to update its array mappings.

class IComponentArray{
public:
	virtual ~IComponentArray() = default;
	virtual void entityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray{
public:
	void insertData(Entity entity, T component){
		assert(m_entityToIndexMap.find(entity) == m_entityToIndexMap.end() && "Component added to same entity more than once.");

		// Put the new entry at the end and update the maps
		size_t newIndex = m_size;
		m_entityToIndexMap[entity] = newIndex;
		m_indexToEntityMap[newIndex] = entity;
		m_componentArray[newIndex] = component;
		++m_size;
	}

	void removeData(Entity entity){
		assert(m_entityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
		size_t indexOfLastElement = m_size - 1 ;
		m_componentArray[indexOfRemovedEntity] = m_componentArray[indexOfLastElement];

		// Update map to point to moved spot
		Entity entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
		m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		m_entityToIndexMap.erase(entity);
		m_indexToEntityMap.erase(indexOfLastElement);

		--m_size;
	}

	T& getData(Entity entity){
		assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Retrieving non-existent component.");

		// Return a reference to the entity's component
		return m_componentArray[m_entityToIndexMap[entity]];
	}

	void entityDestroyed(Entity entity) override{
		if(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end()){
			removeData(entity);
		}
	}
private:

	// The packed array of components (of generic type T),
	// set to a specified maximum amount, matching the maximum number
	// of entities allowed to exist simultaneously, so that each entity
	// has a unique spot.
	std::array<T, MAX_ENTITIES> m_componentArray;

	// Map from an entity ID to an array index.
	std::unordered_map<Entity, size_t> m_entityToIndexMap;

	// Map from an array index to an entity ID.
	std::unordered_map<size_t, Entity> m_indexToEntityMap;

	// Total size of valid entries in the array.
	size_t m_size;
};

class ComponentManager{
public:
	template<typename T>
	void registerComponent(){
		const char * typeName = typeid(T).name();

		assert(m_componentTypes.find(typeName) == m_componentTypes.end() && "Registering component type more than once.");

		// Add this component type to the component type map
		m_componentTypes.insert({typeName, m_nextComponentType});

		// Create a component array pointer and add it to the component arrays map
		m_componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

		// Increment the value so that the next component registered will be different
		++m_nextComponentType;
	}

	template<typename T>
	ComponentType getComponentType(){
		const char * typeName = typeid(T).name();

		assert(m_componentTypes.find(typeName) == m_componentTypes.end() && "Registering component type more than once.");

		// Return this component's type - used for creating signatures
		return m_componentTypes[typeName];
	}

	template<typename T>
	void addComponent(Entity entity, T component){
		getComponentArray<T>()->insertData(entity, component);
	}

	template<typename T>
	void removeComponent(){
		getComponentArray<T>()->removeData(entity, component);	
	}

	template<typename T>
	T& getComponent(Entity entity){
		// Get a reference to a component from the array for an entity
		return getComponentArray<T>()->getData(entity);
	}

	void entityDestroyed(Entity entity){
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
		for (auto const & pair : m_componentArrays){
			auto const & component = pair.second;

			component->entityDestroyed(entity);
		}
	}
private:
	// Map from the type string pointer to a component type
	std::unordered_map<const char*, ComponentType> m_componentTypes{};

	// Map from type string pointer to a component array
	std::unordered_map<const char *, std::shared_ptr<IComponentArray>> m_componentArrays{};
	
	// The component type to be assigned to the next registered component - starting at 0
	ComponentType m_nextComponentType{};

	// Convenience function to get the statically casted pointer to the ComponentArray of type T
	template<typename T>
	std::shared_ptr<ComponentArray<T>> getComponentArray(){
		const char * typeName typeid(T).name();

		assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
	}
};

class System{
public:
	std::set<Entity> m_entities;
};

class SystemManager{
public:
	template<typename T>
	std::shared_ptr<T> registerSystem(){
		const char * typeName = typeid(T).name();

		assert(m_systems.find(typeName) == m_systems.end() && "Registering system more than once.");

		// Create a pointer to the system and return it so it can be used externally
		auto system = std::make_shared<T>();
		m_systems.insert({typeName, system});
		return system;
	}

	template<typename T>
	void setSignature(Signature signature){
		const char * typeName = typeid(T).name();
		assert(m_systems.find(typeName) != m_systems.end() && "Registering system more than once.");

		// Set the signature for this system
		m_signatures.insert({typeName, signature});
	}

	void entityDestroyed(Entity entity){
		// Erase a destroyed entity from all system list
		// m_entities is a set so no check needed

		for (auto const& pair : m_systems){
			auto const& system = pair.second;

			system->m_entities.erase(entity);
		}
	}

	void entitySignatureChanged(Entity entity, Signature entitySignature){
		// Notify each system that an entity's signature changed
		for(auto const & pair : m_systems){

			auto const & type = pair.first;
			auto const & system = pair.second;
			auto const & systemSignature = m_signatures[type];

			// Entity signature matches system signature - insert into set
			if((entitySignature & systemSignature) == systemSignature){
				system->m_entities.insert(entity);
			}
			else{
				system->m_entities.erase(entity);
			}
		}
	}
private:
	// Map from system type string pointer to a signature
	std::unordered_map<const char *, Signature> m_signatures{};

	// Map from system type string pointer to a system pointer
	std::unordered_map<const char *, std::shared_ptr<System>> m_systems{};
};

class Coordinator{
public:
	void init(){
		// Create pointers to each manager
		m_componentManager = std::make_unique<ComponentManager>();
		m_entityManager = std::make_unique<EntityManager>();
		m_systemManager = std::make_unique<SystemManager>();
	}

	Entity createEntity{
		return m_entityManager->createEntity();
	}

	void destroyEntity(Entity entity){
		m_entityManager->destroyEntity(entity); 
	}

	// Copmonent methods
	template<typename T>
	void registerCopmonent(){
		m_componentManager->registerComponent<T>();
	}

	template<typename T>
	void addComponent(Entity entity, T component){
		m_componentManager->addComponent<T>(entity, component);

		auto signature = m_entityManager->getSignature(entity);
		signature.set(m_componentManager->getComponentType<T>(), true);
		m_entityManager->setSignature(entity, signature);

		m_systemSignature->entitySignatureChanged(entity, signature);
	}

	template<typename T>
	void removeComponent(Entity entity){
		m_componentManager->removeComponent<T>(entity);

		auto signature = m_entityManager->getSignature(entity);
		signature.set(m_componentManager->getComponentType<T>(), false);
		m_entityManager->setSginature(entity, signature);

		m_systemManager->entitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& getComponent(Entity entity){
		return m_componentManager->getComponent<T>(entity);
	}

	template<typename T>
	ComponentType getComponentType(){
		return m_componentManager->getComponentType<T>();
	}

	// System methods
	template<typename T>
	std::sahred_ptr<T> registerSystem(){
		return m_systemManager->registerSystem<T>();
	}

	template<typename T>
	void setSystemSignature(Signature signature){
		m_systemManager->setSignature<T>(signature);
	}
private:
	std::unique_ptr<ComponentManager> m_componentManager;
	std::unique_ptr<EntityManager> m_entityManager;
	std::unique_ptr<SystemManager> m_systemManager;
};

extern Coordinator g_coordinator;