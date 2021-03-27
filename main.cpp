#include <util.h>
#include <bootinfo.h>
#include <kernel_info.h>
#include <config.h>

#include <renderer/font_renderer.h>
#include <renderer/mouse_renderer.h>

#include <paging/page_table_manager.h>

#include <interrupts/panic.h>

#include <driver/driver.h>
#include <driver/keyboard.h>
#include <driver/mouse.h>
#include <driver/driver.h>
#include <driver/serial.h>

#include <fe/fe_runner.h>

class PrintfKeyboardEventHandler : public driver::KeyboardEventHandler{
	public:
		void KeyDown(char c){
			renderer::global_font_renderer->printf("%c", c);
		}
};

class MouseRendererMouseEventHandler : public driver::MouseEventHandler{
	public:
		void OnMouseDown(uint8_t button) {
			renderer::global_mouse_renderer->on_mouse_down(button);
		}

		void OnMouseMove(uint8_t mouse_packet[4]) {
			renderer::global_mouse_renderer->on_mouse_move(mouse_packet);
		}
};

extern "C" void _start(bootinfo_t* bootinfo) {
	KernelInfo kernel_info = init_kernel(bootinfo);
	PageTableManager* page_table_manager = kernel_info.page_table_manager;

	renderer::global_font_renderer->printf("\nFoxOS version %s %d Copyright (C) 2021 %s\n", RELEASE_T, VERSION, VENDOR);
	renderer::global_font_renderer->printf("This program comes with ABSOLUTELY NO WARRANTY.\n");
	renderer::global_font_renderer->printf("This is free software, and you are welcome to redistribute it.\n\n");

	renderer::global_font_renderer->printf("Im now colored %fString: %s, Hex: 0x%x, Dec: %d, Char: %c %rand now im white\n\n", 0xff00ff00, "Hello World!", 0xf00d, 1001, 'X');

	PrintfKeyboardEventHandler kbhandler;
	driver::KeyboardDriver keyboard_driver(&kbhandler);

	driver::global_driver_manager->add_driver(&keyboard_driver);

	MouseRendererMouseEventHandler mhandler;
	driver::MouseDriver mouse_driver(&mhandler);

	driver::global_driver_manager->add_driver(&mouse_driver);

	renderer::global_font_renderer->printf("RSDP: %f0x%x%r\n", 0xffff00ff, bootinfo->rsdp);

	driver::global_driver_manager->activate_all(false);


	renderer::global_font_renderer->printf("Running fe now :D\n");

	FeRunner runner;
	runner.run_code((char*) "( = reverse (fn (lst) (let res nil) (while lst ( = res (cons (car lst) res)) ( = lst (cdr lst))) res)) (= animals '(\"cat\" \"dog\" \"fox\")) (print (reverse animals))");

	while (true) {
		asm ("hlt");
	}

}