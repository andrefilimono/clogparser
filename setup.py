from setuptools import setup, Extension

setup(
    name="clogparser",
    version="0.0.1",
    description='Log parser for Python written in C',
    license="MIT",
    ext_modules=[Extension("clogparser", ["clogparser.c"])],
    test_suite='tests',
    classifiers=[
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
        'Programming Language :: Python',
        'Programming Language :: Python :: 2',
        'Programming Language :: Python :: 2.6',
        'Programming Language :: Python :: 2.7',
        'Programming Language :: Python :: 3',
        'Topic :: Software Development :: Libraries :: Python Modules'
    ]
)
