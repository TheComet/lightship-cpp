#include "lightship/Player.h"
#include "lightship/GameConfig.h"
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Scene.h>


#include <iostream>

using namespace Urho3D;

// ----------------------------------------------------------------------------
Player::Player(Context* context) :
    Component(context)
{
}

// ----------------------------------------------------------------------------
void Player::RegisterObject(Urho3D::Context* context)
{
    context->RegisterFactory<Player>("Lightship");
}

// ----------------------------------------------------------------------------
void Player::OnNodeSet(Node* node)
{
    if (node == NULL && modelNode_ != NULL)
    {
        modelNode_->Remove();
        modelNode_ = NULL;

        UnsubscribeFromEvent(E_UPDATE);
    }
    else
    {
        modelNode_ = node->CreateChild("Player Model");
        ResourceCache* cache = GetSubsystem<ResourceCache>();
        StaticModel* model = modelNode_->CreateComponent<StaticModel>();
        model->SetModel(cache->GetResource<Model>("Models/Player.mdl"));
        model->SetMaterial(cache->GetResource<Material>("Materials/Player.xml"));

        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Player, HandleUpdate));
    }
}

// ----------------------------------------------------------------------------
void Player::HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData)
{
    using namespace Update;
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    GameConfig* config = GetSubsystem<GameConfig>();
    node_->SetRotation(Quaternion(
        config->player.rotate.tilt,
        node_->GetRotation().YawAngle() + config->player.rotate.speed * timeStep,
        0
    ));
}
