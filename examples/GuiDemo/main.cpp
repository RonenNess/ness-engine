/*
* NessEngine hello world application. does nothing but rendering "hello world" on the screen.
* PLEASE NOTE: this project relays on the folder examples/ness-engine to be one step above the project dir. so make sure you include it as well.
*				also, the vs project adds the libs dir to the PATH variable when running debug/release. so if you want to run the exectuables outside 
*				visual studio (by clicking on the exectuable), you'll need to copy the dll files to the same dir as the exe.
* Author: Ronen Ness
* Since: 07/2014
*/

#include <NessEngine.h>

int _tmain(int argc, char* argv[])
{
	// init and create a renderer
	Ness::init();
	Ness::Renderer renderer("NOT READY YET DON'T LOOK AT THIS EXAMPLE!!!!", Ness::Sizei(800, 600)); //, Ness::DEFAULT_WINDOW_FLAGS_FULLSCREEN);
	renderer.set_background_color(Ness::Color(0.1f, 0.2f, 0.2f));

	// create a cursor sprite
	renderer.show_cursor(false);
	Ness::SpritePtr cursor = ness_make_ptr<Ness::Sprite>(&renderer, "cursor.png");
	cursor->set_blend_mode(Ness::BLEND_MODE_BLEND);

	// create a new gui manager
	Ness::Gui::GuiManagerPtr gui = renderer.create_gui_manager("gui");
	
	// create the main menu container and dock to the center of the screen
	Ness::Gui::ContainerPtr main_menu = gui->create_container(Ness::Point(6, 5));
	main_menu->dock_to(Ness::Gui::DOCK_CENTER);

	// create a header with background on top of the menu
	Ness::Gui::LabelPtr header = main_menu->create_label("Main Menu");
	header->set_position(Ness::Point(0.0f, -65.0f), Ness::Point::HALF);
	header->set_alignment(Ness::TEXT_ALIGN_CENTER);
	header->set_background_image("gui/header_background.png", true);
	header->get_background_image()->add_scale(1.0f);
	header->get_background_image()->add_position(Ness::Point(0.0f, -14.0f));

	// create another label with text
	Ness::Gui::LabelPtr welcome = main_menu->create_label("Welcome to GUI example!");
	welcome->set_alignment(Ness::TEXT_ALIGN_CENTER);

	Ness::CameraPtr camera = renderer.create_camera();

	// create the events handler
	Ness::Utils::EventsPoller EventsPoller;
	Ness::Utils::ApplicationEvents app;
	Ness::Utils::Mouse mouse;
	EventsPoller.add_handler(*gui.get());
	EventsPoller.add_handler(app);
	EventsPoller.add_handler(mouse);

	// loop until exit button is pressed
	while( !app.got_quit() )
	{
		// handle events
		EventsPoller.poll_events();

		// set cursor state
		if (mouse.is_down(Ness::MOUSE_LEFT))
		{
			cursor->set_source_from_sprite_sheet(Ness::Pointi(1, 0), Ness::Pointi(2, 1), true);
		}
		else
		{
			cursor->set_source_from_sprite_sheet(Ness::Pointi(0, 0), Ness::Pointi(2, 1), true);
		}

		// render the scene
		renderer.start_frame();
		gui->render();
		cursor->set_position(mouse.position());
		cursor->render(camera);
		renderer.end_frame();
	}

	// finish
	return 0;
}