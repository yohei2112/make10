#include "TwitterUtil.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

#define CLASS_NAME "net/yohei2112/make10/make10"

using namespace std;
using namespace cocos2d;


void TwitterUtil::openTweetDialog(char const* tweet)
{
    JniMethodInfo methodInfo;
    
    if (!JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME,"openTweetDialog", "(Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring stringArg = methodInfo.env->NewStringUTF(tweet);
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
    methodInfo.env->DeleteLocalRef(stringArg);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}
