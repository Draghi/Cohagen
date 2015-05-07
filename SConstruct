import os
import sys

env = Environment(tools = ['mingw'], CC = 'gcc', ENV = os.environ)
env.Append(CPPPATH = './src/')
env.Append(LIBPATH = './out/lib/')
env.Append(CFLAGS = ['-Wall', '-pedantic', '-std=c11', '-ggdb'])

platform = sys.platform
if platform == "win32":
	env.Append(LIBS = ['opengl32','m', 'glfw3'])
else:
	env.Append(LIBS = ['libglfw', 'GL', 'm'])

#Set scons to output object files to the "build" directory.
env.VariantDir('./build', './src', duplicate=0)
sources = [Glob("./build/*.c"), Glob("./build/*/*.c")]

object_list = env.Object(source = sources)

env.Program(target="./out/bin/coh", source=object_list)

