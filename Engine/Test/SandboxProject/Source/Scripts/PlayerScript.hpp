#include "Script/NativeScript.hpp"
#include "CustomEnum.hpp"

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
        Field<float32, CS_FIELD_NAME("Gravity")>
			mGravity = 9.8f;

        Field<float32, CS_FIELD_NAME("Movement Multiplier")>
            mMovementMultiplier = 1.0f;

        Field<float3, CS_FIELD_NAME("Velocity")>
            mVelocity = float3 { 0.0f, 0.0f, 0.0f };
    };

}
