#include "Script/NativeScript.hpp"

namespace Cosmic
{

    class PlayerScript : public NativeScript
    {
    public:
        PlayerScript(Entity entity);

    public:
        virtual void OnInstantiate() override;        
    };

    
}
