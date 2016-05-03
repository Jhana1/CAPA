#! /usr/bin/env python

import os
import shutil

__python_package_dir = os.path.dirname(os.path.abspath(__file__))
scripts_dir = os.path.dirname(__python_package_dir)
root_dir = os.path.dirname(scripts_dir)
build_root = os.path.join(root_dir, "build")

def module_source_dir(module_name):
    return os.path.join(root_dir, module_name)

def CAPA_module_source_dir(module_name):
    return module_source_dir("CAPA-" + module_name)

def module_build_dir(module_name):
    return os.path.join(build_root, module_name)

def CAPA_module_build_dir(module_name):
    return module_build_dir("CAPA-" + module_name)

def CAPA_module_dogfooding_dir(module_name):
    return module_build_dir("CAPA-" + module_name + "-dogfooding")

class source:
    clang_dir = module_source_dir("llvm")
    googletest_dir = module_source_dir("googletest")

    core_dir = CAPA_module_source_dir("core")
    metrics_dir = CAPA_module_source_dir("metrics")
    rules_dir = CAPA_module_source_dir("rules")
    reporters_dir = CAPA_module_source_dir("reporters")
    driver_dir = CAPA_module_source_dir("driver")

    json_compilation_database_dir = CAPA_module_source_dir("json-compilation-database")
    xcodebuild_dir = CAPA_module_source_dir("xcodebuild")

class build:
    clang_build_dir = module_build_dir('llvm')
    clang_install_dir = module_build_dir("llvm-install")
    googletest_build_dir = module_build_dir("googletest")

    core_build_dir = CAPA_module_build_dir("core")
    metrics_build_dir = CAPA_module_build_dir("metrics")
    rules_build_dir = CAPA_module_build_dir("rules")
    reporters_build_dir = CAPA_module_build_dir("reporters")
    driver_build_dir = CAPA_module_build_dir("driver")

    bundle_dir = CAPA_module_build_dir("release")

    core_dogfooding_dir = CAPA_module_dogfooding_dir("core")
    metrics_dogfooding_dir = CAPA_module_dogfooding_dir("metrics")
    rules_dogfooding_dir = CAPA_module_dogfooding_dir("rules")
    reporters_dogfooding_dir = CAPA_module_dogfooding_dir("reporters")
    driver_dogfooding_dir = CAPA_module_dogfooding_dir("driver")

class url:
    llvm = 'http://llvm.org/svn/llvm-project/llvm/'
    clang = 'http://llvm.org/svn/llvm-project/cfe/'
    clang_rt = 'http://llvm.org/svn/llvm-project/compiler-rt/'
    googletest = 'http://googlemock.googlecode.com/svn/trunk/'

    json_compilation_database = 'https://github.com/jhana1/CAPA-json-compilation-database.git'
    xcodebuild = 'https://github.com/jhana1/CAPA-xcodebuild.git'

    clang_prebuilt_binary_for_darwin = 'http://llvm.org/releases/3.7.0/clang+llvm-3.7.0-x86_64-apple-darwin.tar.xz'
    clang_prebuilt_binary_for_ubuntu_lts = 'http://llvm.org/releases/3.7.0/clang+llvm-3.7.0-x86_64-linux-gnu-ubuntu-14.04.tar.xz'

def cd(dir_path):
    os.chdir(dir_path)

def mv(src_path, dst_path):
    os.rename(src_path, dst_path)

def rm_f(dir_path):
    if os.path.isdir(dir_path):
        shutil.rmtree(dir_path)

def mkdir_p(dir_path):
    if not os.path.isdir(dir_path):
        os.makedirs(dir_path)

def cp(src_path, dst_path):
    shutil.copy2(src_path, dst_path)

def cp_r(src_path, dst_path):
    shutil.copytree(src_path, dst_path)

