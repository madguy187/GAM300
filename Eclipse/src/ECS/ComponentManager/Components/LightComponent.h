#ifndef LIGHTCOMPONENT_H
#define LIGHTCOMPONENT_H

namespace Eclipse
{
    struct LightComponent
    {
        META_DATA(LightComponent);
        // Darren -
        // Take not this is a dirty Component and im just gonna use it for tracking of lights
        // and iterating in my lighting system
        bool Render = false;
    };
}
#endif /* LIGHTCOMPONENT_H */
