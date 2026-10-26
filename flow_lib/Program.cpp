#include "flow_asset_database.h"
#include "flow_asset_registry.h"
#include "flow_color.h"
#include "flow_fwd.h"
#include "flow_io.h"
#include "flow_material.h"
#include "flow_mesh.h"
#include "flow_model.h"
#include "flow_obj_loader.h"
#include "flow_sprite_loader.h"
#include "flow_sprite.h"
#include "flow_transform.h"
#include "flow_util.h"
#include "flow_vector2.h"
#include "flow_vector3.h"
#include "flow_vertex.h"

using namespace flow::io;

int main(){

    Console::WriteLine("this is a test Progam.cpp", ConsoleEffect(255,255,255, 0,0,0, {CONSOLE_STYLE_ITALIC, CONSOLE_STYLE_BLINK}));

    Console::Write("input:"); auto input = Console::ReadLine();

    Console::WriteLine("input was: " + input);

    return 0;
}