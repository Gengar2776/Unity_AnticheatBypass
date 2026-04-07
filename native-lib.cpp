#include <jni.h>
#include <string>
#include <thread>
#include <chrono>
#include <BNM/Class.hpp>
#include <BNM/Method.hpp>
#include <BNM/Field.hpp>
#include "BNM/Loading.hpp"
#include "include/BNMIncludes.hpp"
#include "include/Main/VRImGuiMenu.h"




static void (*orig_quit)() = nullptr;
static void (*orig_quit_int)(int) = nullptr;


void newQuitHook() {

    // do nothing
}

void newQuitHook_int(int code) {


}


void HookQuit() {

    auto klass = BNM::Class("UnityEngine", "Application",
                            BNM::Image("UnityEngine.CoreModule.dll"));


    auto quitMethod = klass.GetMethod("Quit", 0);
    if (quitMethod.IsValid()) {
        orig_quit = (void(*)())quitMethod.GetInfo();
        BNM::VirtualHook(quitMethod, (void*)newQuitHook);
        // if you have evrrors with virtualhook call a diff hook like Invoke its way better
        // also this depends on your BNM version

    }


    auto quitMethodInt = klass.GetMethod("Quit", 1);
    if (quitMethodInt.IsValid()) {
        orig_quit_int = (void(*)(int))quitMethodInt.GetInfo();
        VirtualHook(quitMethodInt, (void*)newQuitHook_int);

    }
}







void (*Awake)(void*);
void new_Awake(void* instance) {
    Awake(instance);
    BNM_LOG_INFO("Menu Initialization Called");
    VRMenu::onupdate();
}

void (*LateUpdate)(void*);
void new_LateUpdate(void* instance) {
    LateUpdate(instance);
}

void hook() {
    BNM_LOG_DEBUG(O("Start hooking"));

    auto GorillaLocomotion = Class(BNM_OBFUSCATE("GorillaLocomotion"), BNM_OBFUSCATE("Player"));
    if (GorillaLocomotion) {
        InvokeHook(GorillaLocomotion.GetMethod(O("LateUpdate")), new_LateUpdate, LateUpdate);
        InvokeHook(GorillaLocomotion.GetMethod(O("Awake")), new_Awake, Awake);
    }
}

void loaded() {
    BNM_LOG_INFO(O("initialized bnm v%s"), BNM_VER);
    hook();
}

__attribute__((constructor))
void init_main() {
    BNM::Loading::AllowLateInitHook();
    std::thread([]() {
        void* handle = nullptr;
        while (!handle) {
            handle = BNM_dlopen("libil2cpp.so", RTLD_LAZY);
            if (!handle) std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
        BNM::Loading::AddOnLoadedEvent(loaded);
        BNM::Loading::TryLoadByDlfcnHandle(handle);
    }).detach();
}