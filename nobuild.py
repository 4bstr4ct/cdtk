import os
import sys
import argparse
import json


def _clear_builds():
	build_directory = os.path.join(os.getcwd(), 'build')
	if os.path.isdir(build_directory):
		import shutil
		shutil.rmtree(build_directory)
		os.mkdir(build_directory)


def _clear_terminal():
	os.system('cls' if os.name == 'nt' else 'clear')


def _build_and_run_test(test):
	test_name = test['name']

	sources = ''
	for source in test['sources']:
		sources += ' ' + os.path.join(os.getcwd(), source)

	compiler = test['compiler']

	build_args = ''
	for build_arg in test['buildArgs']:
		build_args += ' ' + build_arg

	run_args = ''
	for run_arg in test['runArgs']:
		run_args += ' ' + run_arg

	dependencies = ''
	for dependency in test['dependencies']:
		dependencies += ' ' + os.path.join(os.getcwd(), dependency)

	build_directory = os.path.join(os.getcwd(), 'build')
	if not os.path.isdir(build_directory):
		os.mkdir(build_directory)

	build_tests_directory = os.path.join(build_directory, 'tests')
	if not os.path.isdir(build_tests_directory):
		os.mkdir(build_tests_directory)

	output_file_path = os.path.join(build_tests_directory, '{}.out'.format(test_name))

	if sys.platform == 'win32':
		output_file_path = '{}.exe'.format(output_file_path)
		os.system('cmd /C \"{} {} -o {} {} {}\"'.format(
			compiler,
			build_args,
			output_file_path,
			sources,
			dependencies))
		os.system('cmd /C \"{}\"'.format(output_file_path))
	else:
		os.system('{} {} -o {} {} {}'.format(
			compiler,
			build_args,
			output_file_path,
			sources,
			dependencies))
		os.system('{}'.format(output_file_path))


def _main():
	root_parser = argparse.ArgumentParser(prog='test')
	root_parser.add_argument(
		'--config',
		type=str,
		required=True,
		help='A config file for the builder and runner'
	)

	root_parser.add_argument(
		'--clear-builds',
		action='store_true',
		help='Clear all builds'
	)

	root_parser.add_argument(
		'--clear-terminal',
		action='store_true',
		help='Clear terminal'
	)

	args = vars(root_parser.parse_args())

	full_config_file_path = os.path.join(os.getcwd(), args['config'])
	if not os.path.isfile(full_config_file_path):
		print('Provided config file path {} was not found!'.format(full_config_file_path))
		exit(1)

	config_content = None
	with open(full_config_file_path) as file:
		config_content = json.load(file)

	if config_content is None:
		print('Failed to parse config file!')
		exit(1)

	config_tests_content = config_content['tests']

	if args['clear_builds']:
		_clear_builds()

	if args['clear_terminal']:
		_clear_terminal()

	for test in config_tests_content:
		_build_and_run_test(test)

	print('Finished building tests. Quitting...')
	exit(0)


_main()
