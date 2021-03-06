import sys
import os
import shutil
import yaml
import platform
import subprocess

def existed(path):
	return os.path.exists(path)

def create_dir(dir_name):
	if not existed(dir_name):
		os.mkdir(dir_name)
		
def remove_dir(dir_name):
	if existed(dir_name):
		shutil.rmtree(dir_name)

def create_new_dir(dir_name):
	remove_dir(dir_name)
	create_dir(dir_name)

def run_cmd(cmd_str):
	print("--run cmd--%s"%cmd_str)
	# rs = os.popen(cmd_str, "r")
	# cmdout = rs.read()
	# print cmdout

	poll_code = None
	p = None
	try:
		p = subprocess.Popen(cmd_str, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, bufsize=0)
		while True:
			output = p.stdout.readline()
			if output:
				print(output),
			poll_code = p.poll()
			if poll_code is not None:
				break
	except Exception as e:
		p.terminate()
		print(e)
	finally:
		p.stdout.flush()
		output = p.stdout.read()
		if output:
			print(output)
	# assert poll_code == 0, 'run cmd failed, code %s!' % poll_code


# load conf from yaml
conf = None
with open("thirdparty.yaml") as f:
	try:
		conf = yaml.load(f)
	except yaml.YAMLError as e:
		print(e)

assert conf is not None
print("conf:%s"%conf)
                         
# check os type
os_name = platform.system().lower()
print("os_name", os_name)
systems = {"windows": "win", "darwin": "osx", "linux" : "linux"}
for k, v in systems.items():
	if os_name in k:
		current_os = v
		break
else:
	print("os_name %s error" % os_name)
	
print('current_os:%s'%current_os)

# create dir
src_dir = os.path.realpath(conf.get("src", "src"))
build_dir = os.path.realpath(conf.get("build", "build"))
install_dir = os.path.realpath(conf.get("install", "install"))
print('src_dir:%s'%src_dir)
print('build_dir:%s'%build_dir)
print('install_dir:%s'%install_dir)

create_dir(src_dir)
create_dir(build_dir)
create_dir(install_dir)

# download libs
def download_libs():
	for name, attr in conf["thirdparty"].items():
		git_url = attr.get("git", "")
		if git_url:
			source_path = os.path.join(src_dir, name)
			if not existed(source_path):
				cmd_str = "cd %s && git clone %s %s"%(src_dir, git_url, name)
				run_cmd(cmd_str)
				if attr.get('tag'):
					cmd_str = "cd %s && git checkout %s"%(source_path, attr.get('tag'))
					run_cmd(cmd_str)
				cmd_str = "cd %s && git submodule update --init --recursive"%source_path
				run_cmd(cmd_str)

# build ninja
ninja = None

def build_ninja():
	global ninja
	ninja_src_dir = os.path.join(src_dir, "ninja")
	ninja_build_dir = os.path.join(build_dir, "ninja")
	suffix = ".exe" if current_os == "win" else ""
	ninja = os.path.join(ninja_build_dir, "ninja%s"%(suffix, ))
	if existed(ninja):
		print("ninja is ready, skipped")
		return
	create_new_dir(ninja_build_dir)
	cmd_str = "cd %s && python %s/configure.py --bootstrap" % (ninja_build_dir, ninja_src_dir)
	run_cmd(cmd_str)

# build other libs
def build_libs():
	for name, attr in conf["thirdparty"].items():
		if name == "ninja":
			continue
		build_one_lib(name, attr)

def build_one_lib(lib_name, attr):
	lib_src_dir = os.path.join(src_dir, lib_name)
	lib_build_dir = os.path.join(build_dir, lib_name)

	create_new_dir(lib_build_dir)

	now_path = os.path.realpath(".")
	os.chdir(lib_build_dir)
	print("-- build %s begin --"%lib_name)
	final_cmake_args = []
	final_cmake_args.extend(conf.get("cmake_args", []))

	cmake_args = attr.get('cmake_args', None)
	if cmake_args:
		final_cmake_args.extend(cmake_args)
	final_cmake_args.append("-DCMAKE_INSTALL_PREFIX=%s" % install_dir)
	final_cmake_args.append("-DCMAKE_MAKE_PROGRAM=%s" % ninja)
	cmake_arg_str = " ".join(final_cmake_args)

	cmakelist_dir = attr.get('cmakelist_dir')
	if cmakelist_dir:
		lib_src_dir = os.path.join(lib_src_dir, cmakelist_dir)
	cmd_str = " ".join(["cmake -G Ninja", cmake_arg_str, lib_src_dir])
	run_cmd(cmd_str)
	run_cmd(ninja)
	run_cmd(ninja + " install")
	print("-- build %s end --" % lib_name)
	os.chdir(now_path)

# clean
def clean_build():
	create_new_dir(build_dir)

if __name__ == "__main__":
	if len(sys.argv) > 1:
		if sys.argv[1] == "clean":
			clean_build()
			exit(0)
	else:
		download_libs()
		build_ninja()
		build_libs()
