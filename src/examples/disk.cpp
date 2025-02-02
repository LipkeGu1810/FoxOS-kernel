#include <driver/disk/disk.h>

#include <paging/page_frame_allocator.h>

#include <driver/serial.h>

void disk_test() {
	char* buffer = (char*) global_allocator.request_page(); //Request a page from the page frame allocator to store the data read by the disk manager
	driver::disk::global_disk_manager->read(0, 0, 1, buffer);//Read raw data from the disk of ID 0 into the buffer

	for (int t = 0; t < 512; t++){
		driver::global_serial_driver->printf("%c", buffer[t]); //Print part of the buffer's contents
	}
	driver::global_serial_driver->printf("\n");

	global_allocator.free_page(buffer); //Free the requested page
}