#include "Plugin.h"

#include <string>
#include <jni.h>

#ifdef LUA_DEFINE
# undef LUA_DEFINE
#endif
#define LUA_DEFINE(name) Define(#name, [](lua_State *L) -> int


Plugin::Plugin()
{
    static std::string optionString = "-Djava.class.path=/usr/lib/java";
    JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */
    auto* options = new JavaVMOption[1];
    options[0].optionString = const_cast<char *>(optionString.c_str());
    vm_args.version = JNI_VERSION_1_8;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;

    JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    // Get system class
    jclass syscls = env->FindClass("java/lang/System");
// Lookup the "out" field
    jfieldID fid = env->GetStaticFieldID(syscls, "out", "Ljava/io/PrintStream;");
    jobject out = env->GetStaticObjectField(syscls, fid);
// Get PrintStream class
    jclass pscls = env->FindClass("java/io/PrintStream");
// Lookup printLn(String)
    jmethodID mid = env->GetMethodID(pscls, "println", "(Ljava/lang/String;)V");
// Invoke the method
    jstring str = env->NewStringUTF("Hello World!");
    env->CallVoidMethod(out, mid, str);
}

JNIEXPORT void JNICALL Java_LuaAccess_callVoid(JNIEnv *env, jobject obj, jstring string) {
    
}

Plugin::~Plugin()
{
    jvm->DestroyJavaVM();
}