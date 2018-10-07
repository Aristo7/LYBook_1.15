#include "MyProject_precompiled.h"
#include "MyScriptSystemComponent.h"
#include <AzCore/Serialization/EditContext.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <ISystem.h>
#include <ScriptCanvas/MyNodeLibrary.h>

using namespace MyProject;

void MyScriptSystemComponent::Reflect(AZ::ReflectContext* rc)
{
    if (auto* sc = azrtti_cast<AZ::SerializeContext*>(rc))
    {
        sc->Class<MyScriptSystemComponent, Component>()
            ->Version(1);
        if (AZ::EditContext* ec = sc->GetEditContext())
        {
            using namespace AZ::Edit::Attributes;
            using namespace AZ::Edit::UIHandlers;
            ec->Class<MyScriptSystemComponent>(
                "My Script System Component",
                "[various helpful tool for scripts]")
                ->ClassElement(
                    AZ::Edit::ClassElements::EditorData, "")
                // mark it as a system component
                ->Attribute(AppearsInAddComponentMenu,
                    AZ_CRC("System", 0xc94d118b));
        }
    }
    if (auto* bc = azrtti_cast<AZ::BehaviorContext*>(rc))
    {
        bc->EBus<MyHelperBus>("MyHelperBus")->
            Event("IsEditor", &MyHelperBus::Events::IsEditor);
    }
    MyNodeLibrary::Reflect(rc);
}

void MyScriptSystemComponent::Init()
{
    using namespace ScriptCanvas;
    auto nodeRegistryVariable = AZ::Environment::
        FindVariable<NodeRegistry>(s_nodeRegistryName);
    if (nodeRegistryVariable)
    {
        NodeRegistry& nodeRegistry = nodeRegistryVariable.Get();
        MyNodeLibrary::InitNodeRegistry(nodeRegistry);
    }
}

bool MyScriptSystemComponent::IsEditor()
{
    return gEnv && gEnv->IsEditor();
}
