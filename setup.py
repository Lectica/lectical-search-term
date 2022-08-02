
from distutils.core import setup, Extension

setup(
    name="lectica",
    version="0.0.4",
    author="Alessandro Madruga Correia",
    description="Lectica C module to search terms in text.",
    ext_modules=[Extension("lectica",
                           extra_compile_args=["-O3"],
                           sources=["./src/lectica_search.c"])])
