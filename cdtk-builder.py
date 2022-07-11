import os
import argparse
import sys


CDTK_INCLUDE_DIRECTORY = os.path.join(os.getcwd(), "include")
CDTK_TESTS_DIRECTORY = os.path.join(os.getcwd(), "tests")
CDTK_TESTS_OUTPUT_DIRECTORY = os.path.join(os.getcwd(), "build", "tests")


def _copyLib(lib_directory):
	include_directory = os.path.join(lib_directory, "include")
	source_directory = os.path.join(lib_directory, "source")
	headers = os.listdir(include_directory)
	sources = os.listdir(source_directory)

	for header_file, source_file in zip(headers, sources):
		full_header_file_path = os.path.join(include_directory, header_file)
		full_source_file_path = os.path.join(source_directory, source_file)
		header_content = ""
		source_content = ""

		with open(full_header_file_path, "r") as file:
			header_content = file.readlines()

		with open(full_source_file_path, "r") as file:
			source_content = file.readlines()

		for index, line in enumerate(source_content):
			if header_file in line:
				source_content.remove(line)

		with open(os.path.join(CDTK_INCLUDE_DIRECTORY, header_file), "w") as file:
			switchIndex = 0
			for index, line in reversed(list(enumerate(header_content))):
				if line.replace('\n', '') == '#endif':
					switchIndex = index
					break

			file.writelines(header_content[0 : switchIndex])
			file.writelines('\n')
			file.writelines(source_content)
			file.writelines('\n')
			file.writelines(header_content[switchIndex :])


def run_tests():
	built_tests = []
	for file in os.listdir(CDTK_TESTS_DIRECTORY):
		if file.endswith('.c') or file.endswith('.cc'):
			output_file_name = '{}.out'.format(file)

			if sys.platform == "win32":
				output_file_name = '{}.exe'.format(output_file_name)

			full_file_name = os.path.join(CDTK_TESTS_DIRECTORY, file)
			full_output_file_name = os.path.join(CDTK_TESTS_OUTPUT_DIRECTORY, output_file_name)
			os.system('cmd /C \"gcc -std=c11 -o {} {}\"'.format(full_output_file_name, full_file_name))
			built_tests.append(full_output_file_name)

	for file in built_tests:
		os.system('cmd /C \"{}\"'.format(file))


def _main():
	parser = argparse.ArgumentParser()

	parser.add_argument(
		'--build-cdtk',
		type=str,
		required=True,
		help='build cdtk with provided config file'
	)
 
	parser.add_argument(
		'--run-tests',
		action='store_true',
		help='run all tests in root/tests/'
	)

	args = vars(parser.parse_args())

	if args['build_cdtk'] is not '':
		with open(args['build_cdtk'], "r") as file:
			line = file.readline()
			if '#' not in line:
				lib_directory = os.path.join(os.getcwd(), line)
				_copyLib(lib_directory.replace('\n', ''))

	if args['run_tests']:
		run_tests()


_main()
