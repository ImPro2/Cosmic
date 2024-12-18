#include "Script/NativeScript.hpp"

namespace Cosmic
{

    class PlayerScript : public NativeScript
    {
    public:
        PlayerScript(Entity entity);

    public:
        virtual void OnInstantiate() override;
        virtual void OnUpdate(Dt dt) override;

    public:
        Field<float32, CS_FIELD_NAME("MovementSpeed")> mMovementSpeed = 100.0f;
    };

}
