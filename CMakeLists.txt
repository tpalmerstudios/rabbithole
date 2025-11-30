cmake_minimum_required(VERSION 3.16)
project(Rabbit
	VERSION
		0.0.1
	DESCRIPTION
		"A custom, extensible database system designed to store, link, and search relational data across multiple entity types with an interface focused on showing data links on various platforms"
	)

target_compile_options(rabbit PRIVATE -Wall -Wextra -Werror)
add_executable(rabbit src/hello.c)
