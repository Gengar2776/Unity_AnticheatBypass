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



using namespace BNM::IL2CPP




std::vector<std::string> antiNames = {
        "AntiModder","QuestLink","NetworkRig","URL","NoLemonScript","Bow","BasilsAuth","KickIfBanned",
        "LeaderboardManager","QuestScript","kick","kickp","DllChecker","DeviceCheck","ModsFolderChecker",
        "Melonloaderchecker","SuspiciousBehaviourChecker","UnitysAntiCheat","LemonFolderChecker","KSHRAnti",
        "AntiKickTest","KickProtection","PastebinLoader","NoNameDetector","AntiModders","PhotonTrigger",
        "Bullet","VoidGuard","QuitOnCollision","AIbooster","StoreMesh","DllSigmaThing","CheckIfInUnity",
        "BanOnStart","FurnacesAntiCheat","hydrasbasicanticheat","TheGriddy","SnowballSpammy","QuestSentinelProtect",
        "QuestScriptFinder","ModsFolderCheckerV2","joshhsFreeAnticheat","PlayfabManager1","DllCheck",
        "HydrasPrivAntiCheat","HydrasUltimateAntiCheat","GorillaNot","Imposter","KarmasMidAC","LoadObjectsOnStart",
        "UnloadObjectsOnStart","KokoAntiSkid","FoxysAttributeTest","FoxysReallyGoodAnticheat","UabeMethod",
        "SuspiciousBehaviourDetector","Dll","FieldCheatDetector","ADetector'1","PersistentSingleton1",
        "Entitlementcheck","AstreiodsGameManager","GetBanReason","GetBanReasonFromSave","SaveManager",
        "TeleportToBan","VersionChecker","KeyPadEnter","AppEntitlementCheck","SignatureCheck",
        "ProtectedUIInt32","AsteroidSpawner","ProtectedInt16","ProtectedVector4","ProtectedFloat",
        "ProtectedQuaternionPref","ProtectedVector3Int","ProtectedVector2Pref","ProtectedString","Asteroid",
        "CheatingDetectionStatus","AntiCheatMonitor","ProtectedUInt64","QuestSentailProtect","assembliesToCheck",
        "AstreiodsGame","WallHackDetector","SpeedHackDetector","ModTool","GOOBEREER","GlitchMonke",
        "IgottaremanethisitwasmadebyK_S_H_R","Kidsthesedays","veryfyyey","ApkChecker","Antimodders",
        "Code.Stages.Anticheats","AppDeeplinkUI","ChangeCosmetic","coinsScripts","PublicZone","SampleUI",
        "LoginHandle.IsClientLoggedIn","ConntrastStretch","CollisionSounds","Donut","WifiCheck",
        "PlayerMovement01","Funnymods","BloxianAnti","checkere","particallagreducer","timmyfixer",
        "antidll","anti-dll","anti-hack","anti-cheat","anticheat","ownsmodcosmetics","OpenLink",
        "hidemyshitsonofabitch","BadBilly","GorillaQuitBox","yummylemons","yummymelons","yummy","Sgima",
        "tpiffail","gameobj","gaymonster","moveon","NewBehaviourScript","spoopy","CokesAnticheat",
        "HorrorAi","sigma","DiscordWebhookTrigger","coke","youcantgetwomenever","youbroketherules",
        "IsCorrect","yabadabadooo","workrrr","WHYYYYY","WAAAAAA","veryyfyyey","TURN","Treeheehehe",
        "treehe","spin","byebye","byebyeee","rotatething","settingsyoo","thinghehe","treeman","pain",
        "EnableObjectAfterDelay","yeye","handscantbeextremelyfarapart","byeeeeeeeeee","heydonthaveeverything",
        "owner","whyareyoufrozen","no","nametag","Checkn","blablabla","EditorOnlyStuff",
        "somethingtolurethem","NameDetector","ModFolderChecker","AntiCheat",
        "IsModded","DLL Checker","Melonloaderchecker","FolderCheck",
        "AnticheatHider","OPAnticheat","CoolThing","KSHRAnti",
        "LemonLoaderChecker","Mods Folder Checker","AntiModders",
        "AntiCheat","ModderChecker","LemonFolderChecker","QuestLink"
};



void AntiChecker_FindAndDisable() {
    for (auto& name : antiNames) {
        GameObject* obj = GameObject::Find(name.c_str());
        if (obj) {
            BNM_LOG_WARN("made by gengar", name.c_str());
            obj->SetActive(false);
        } else {
            BNM_LOG_WARN("not working")
        }
    }
}






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

//if you use pls give credits this took a little while
