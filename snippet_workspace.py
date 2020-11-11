#!/usr/bin/python3

import os
import stat
import argparse

CPP_DEFAULT_BUILD = "g++ main.cpp -o a.out"

CPP_DEFAULT_MAIN = "\'#include <iostream>\n\nint main(){\n    std::cout<<\"Main.\"<<std::endl;\n}\'"

def make_arg_parser():

    title_msg = "Creates snippet workspace directory."

    parser = argparse.ArgumentParser(description=title_msg,
                                         prog="get_status",
                                         formatter_class=argparse.RawTextHelpFormatter,
                                         epilog=(""))

    parser.add_argument("snippet_workspace",
                        type=str,
                        metavar="%s %s...",
                        help="Snippet workspace name")
    return parser

def main():
	parser = make_arg_parser()
	args = parser.parse_args()
	snippet_workspace = os.path.abspath(args.snippet_workspace)

	if os.path.exists(snippet_workspace):
		raise ValueError("Workspace already exists.")
	else:
		os.mkdir(snippet_workspace)
		main_file_path = os.path.join(snippet_workspace, "main.cpp")
		build_file_path = os.path.join(snippet_workspace, "build")

		os.system("echo {} > {} ".format(CPP_DEFAULT_BUILD, build_file_path))
		os.system("chmod +x {}".format(build_file_path))

		os.system("echo {} > {} ".format(CPP_DEFAULT_MAIN, main_file_path))


if __name__ == '__main__':
	main()