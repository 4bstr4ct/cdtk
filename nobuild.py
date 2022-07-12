import os
import sys
import argparse
import json


def _build_lib_tests(data):
	build_flags = ''
	for build_flag in data['buildArgs']:
		build_flags += ' ' + build_flag

	dependencies = ''
	for file in data['dependencies']:
		full_path = os.path.join(os.getcwd(), file)
		dependencies += ' ' + full_path

	compiler = data['compiler']

	if not os.path.isdir(os.path.join(os.getcwd(), 'build')):
		os.mkdir(os.path.join(os.getcwd(), 'build'))

	if not os.path.isdir(os.path.join(os.getcwd(), data['buildDirectory'])):
		os.mkdir(os.path.join(os.getcwd(), data['buildDirectory']))

	for file in os.listdir(os.path.join(os.getcwd(), data['rootDirectory'], 'tests')):
		full_test_file_path = os.path.join(os.getcwd(), data['rootDirectory'], 'tests', file)
		full_output_file_path = os.path.join(os.getcwd(), data['buildDirectory'], '{}.{}'.format(file, 'out'))
		if sys.platform == 'win32':
			full_output_file_path = '{}.{}'.format(full_output_file_path, 'exe')

		if sys.platform == 'win32':
			os.system('cmd /C \"{} {} -o {} {} {}\"'.format(
				compiler,
				build_flags,
				full_output_file_path,
				full_test_file_path,
				dependencies))
		else:
			os.system('{} {} -o {} {} {}'.format(
				compiler,
				build_flags,
				full_output_file_path,
				full_test_file_path,
				dependencies))


def _run_lib_tests(data):
	for file in os.listdir(os.path.join(os.getcwd(), data['buildDirectory'])):
		full_file_path = os.path.join(os.getcwd(), data['buildDirectory'], file)
		
		full_output_file_path = os.path.join(os.getcwd(), data['buildDirectory'], file)
		if sys.platform == 'win32':
			full_output_file_path = '{}.{}'.format(full_output_file_path, 'exe')

		if sys.platform == 'win32':
			os.system('cmd /C \"{}\"'.format(full_output_file_path))
		else:
			os.system('{}'.format(full_output_file_path))


def _clear_builds():
	build_directory = os.path.join(os.getcwd(), "build")
	if os.path.isdir(build_directory):
		import shutil
		shutil.rmtree(build_directory)
		os.mkdir(build_directory)


def _clear_terminal():
	os.system('cls' if os.name == 'nt' else 'clear')


def _main():
	root_parser = argparse.ArgumentParser(prog='test')
	root_parser.add_argument(
		'--config',
		type=str,
		required=True,
		help='A config file for the builder and runner'
	)

	subparsers = root_parser.add_subparsers(title='subcommands')
	subparsers.dest = 'command'

	builder_parser = subparsers.add_parser('build-tests', help='Build project')
	builder_parser.add_argument(
		'--clear-builds',
		action='store_true',
		help='Flag to clear all build directory'
	)
 
	runner_parser = subparsers.add_parser('run-tests', help='Run tests')
	runner_parser.add_argument(
		'--clear-terminal',
		action='store_true',
		help='Flag to clear terminal before running tests'
	)

	builder_and_runner_parser = subparsers.add_parser('build-and-run-tests', help='Build and run tests')
	builder_and_runner_parser.add_argument(
		'--clear-all',
		action='store_true',
		help='Clear build and terminal screen'
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

	if args['command'] == 'build-tests':
		if args['clear_builds']:
			_clear_builds()

		for lib in config_content:
			_build_lib_tests(lib)

		print('Finished building tests. Quitting...')
		exit(0)

	if args['command'] == 'run-tests':
		if args['clear_terminal']:
			_clear_terminal()

		for lib in config_content:
			_run_lib_tests(lib)

		print('Finished running tests. Quitting...')
		exit(0)

	if args['command'] == 'build-and-run-tests':
		if args['clear_all']:
			_clear_builds()
			_clear_terminal()

		for lib in config_content:
			_build_lib_tests(lib)
			_run_lib_tests(lib)

		print('Finished building and running tests. Quitting...')
		exit(0)


_main()
