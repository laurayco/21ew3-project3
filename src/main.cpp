#include <iostream>
#include <jni.h>
using namespace std;

int main() {
    JavaVM *jvm; // Pointer to the JVM (Java Virtual Machine)
    JNIEnv *env; // Pointer to native interface
    //================== prepare loading of Java VM ============================
    JavaVMInitArgs vm_args;                                                      // Initialization arguments
    JavaVMOption *options = new JavaVMOption[1];                                 // JVM invocation options
    options[0].optionString = (char *)"-Djava.class.path="; // where to find java.class
    vm_args.version = JNI_VERSION_1_6; // minimum Java version
    vm_args.nOptions = 1;                  // number of options
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false; // invalid options make the JVM init fail
    //=============== load and initialize Java VM and JNI interface =============
    jint rc = JNI_CreateJavaVM(&jvm, (void **)&env, &vm_args); // YES !!
    delete options;                                            // we then no longer need the initialisation options.
    if (rc != JNI_OK)
    {
        // TO DO: error processing...
        cin.get();
        exit(EXIT_FAILURE);
    }
    //=============== Display JVM version =======================================
    cout << "JVM load succeeded: Version ";
    jint ver = env->GetVersion();
    cout << ((ver >> 16) & 0x0f) << "." << (ver & 0x0f) << endl;
    jclass cls2 = env->FindClass("HelloJavaWorld"); // try to find the class
    if (cls2 == nullptr)
    {
        cerr << "ERROR: class not found !";
    }
    else
    { // if class found, continue
        cout << "Class MyTest found" << endl;
        jmethodID mid = env->GetStaticMethodID(cls2, "sayhi", "()V"); // find method
        if (mid == nullptr)
            cerr << "ERROR: method not found !" << endl;
        else
        {
            env->CallStaticVoidMethod(cls2, mid); // call method
            cout << endl;
        }
    }
    jvm->DestroyJavaVM();
    return 0;
}