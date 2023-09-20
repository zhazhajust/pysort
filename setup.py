from setuptools import setup, find_packages  
from pybind11.setup_helpers import Pybind11Extension, build_ext

__version__ = "0.0.1"

ext_modules = [
    Pybind11Extension("pysort.particle_sort",
        ["src/datatrack.cpp"],
        # Example: passing in the version to the compiled code
        define_macros = [('VERSION_INFO', __version__)],
        #include_dirs=["/home/yujq/users/caijie/local/hdf5/include", "/opt/intel/compilers_and_libraries_2018.1.163/linux/mpi/intel64/include"],
        #library_dirs=["/home/yujq/users/caijie/local/hdf5/lib"],
        extra_compile_args=["-O3","-fPIC"],
        libraries = ["hdf5"],
        ),
]

setup(  
    name = "pysort",  
    version = __version__,
    keywords = ("pic data reader"),  
    description = "eds sdk",  
    long_description = "eds sdk for python",  
    license = "MIT Licence",

    url = "http://github.com",  
    author = "Jie.Cai",  
    author_email = "jiecai@stu.pku.edu.com",  
    
    packages = find_packages(),  
    include_package_data = True,  
    platforms = "any",  
    install_requires = ["pybind11"],  
  
    ext_modules=ext_modules,
    cmdclass={"build_ext": build_ext},

    scripts = [],  
    entry_points = {  
        'console_scripts': [  
            'test = test.help:main'  
        ]  
    }
)
