#! /usr/bin/env python

import subprocess
import os

from CAPAscripts import path

def git_hash():
    os.chdir(path.root_dir)
    git_hash = subprocess.check_output(['git', 'log', '-n', '1', '--pretty=%h']).split()[0]
    return git_hash

def CAPA_version():
    return "0.10.2"

def CAPA_dev_version():
    return CAPA_version() + '.dev.' + git_hash()

def llvm_branches():
    return ['trunk', llvm_default_branch()]

def llvm_default_branch():
    return 'tags/RELEASE_371/final'
