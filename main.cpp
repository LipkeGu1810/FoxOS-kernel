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
#include <driver/disk/ata.h>
#include <driver/disk/disk.h>

#include <fe/fe_runner.h>

#include <scheduling/pit/pit.h>

#include <shell/shell.h>

class PrintfKeyboardEventHandler : public driver::KeyboardEventHandler{
	public:
		void KeyDown(char c){
			shell::global_shell->keypress(c);
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

	//Keyboard and mouse
	PrintfKeyboardEventHandler kbhandler;
	driver::KeyboardDriver keyboard_driver(&kbhandler);
	driver::global_driver_manager->add_driver(&keyboard_driver);

	MouseRendererMouseEventHandler mhandler;
	driver::MouseDriver mouse_driver(&mhandler);
	driver::global_driver_manager->add_driver(&mouse_driver);

	driver::AdvancedTechnologyAttachment ata0m(true, 0x1F0);
	driver::AdvancedTechnologyAttachment ata0s(false, 0x1F0);
	driver::AdvancedTechnologyAttachment ata1m(true, 0x170);
	driver::AdvancedTechnologyAttachment ata1s(false, 0x170);

	driver::global_driver_manager->add_driver(&ata0m);
	driver::global_driver_manager->add_driver(&ata0s);
	driver::global_driver_manager->add_driver(&ata1m);
	driver::global_driver_manager->add_driver(&ata1s);

	renderer::global_font_renderer->printf("RSDP: %f0x%x%r\n", 0xffff00ff, bootinfo->rsdp);

	driver::global_driver_manager->activate_all(false);

	//fe stuff
	renderer::global_font_renderer->printf("Running fe now :D\n");

	FeRunner runner;

	extern const char fe_push[];
	extern const char fe_reverse[];

	runner.run_code((char*) fe_push);
	runner.run_code((char*) fe_reverse);

	// disk read stuff
	char* buffer = (char*) global_allocator.request_page();

	driver::disk::global_disk_manager->read(0, 0, 1, buffer);

	for (int t = 0; t < 512; t++){
		renderer::global_font_renderer->printf("%c", buffer[t]);
	}

	renderer::global_font_renderer->printf("\n");

	shell::global_shell->init_shell();

	while (true) {
		asm ("hlt");
	}
}