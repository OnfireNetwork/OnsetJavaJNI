#include "Plugin.h"

#include <string>
#include <jni.h>

#ifdef LUA_DEFINE
# undef LUA_DEFINE
#endif
#define LUA_DEFINE(name) Define(#name, [](lua_State *L) -> int


Plugin::Plugin()
{
    JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */
    auto* options = new JavaVMOption[1];
    options[0].optionString = "-Djava.class.path=/usr/lib/java";
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;

    JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    jclass cls = env->FindClass("java/lang/System");
    jfieldID errorField = env->GetStaticFieldID(cls, "err", "(Ljava/io/PrintStream;Ljava/io/PrintStream;II)V");
    jclass printStream = env->FindClass("java/io/PrintStream");
    jmethodID printlnMethod = env->GetMethodID(printStream, "println", "");
    env->CallVoidMethod(env->GetStaticObjectField(cls, errorField), printlnMethod, "Test");
}

Plugin::~Plugin()
{
    jvm->DestroyJavaVM();
}