#include "mainwindow.h"
#include <QApplication>
#include <libusb.h>

static int count = 0;

int hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev,
                     libusb_hotplug_event event, void *user_data) {
  static libusb_device_handle *dev_handle = nullptr;
  struct libusb_device_descriptor desc;
  int rc;

  (void)libusb_get_device_descriptor(dev, &desc);

  if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event) {
    rc = libusb_open(dev, &dev_handle);
    if (LIBUSB_SUCCESS != rc) {
      printf("Could not open USB device\n");
    }
  } else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event) {
    if (dev_handle) {
      libusb_close(dev_handle);
      dev_handle = nullptr;
    }
  } else {
    printf("Unhandled event %d\n", event);
  }
  count++;

  return 0;
}

int main(int argc, char *argv[])
{
    libusb_hotplug_callback_handle callback_handle;
      int rc;

      libusb_init(nullptr);

      rc = libusb_hotplug_register_callback(nullptr, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED ,
                                            LIBUSB_HOTPLUG_NO_FLAGS, 0x05AC, 0x12A8,
                                            LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, nullptr,
                                            &callback_handle);
      if (LIBUSB_SUCCESS != rc) {
        printf("Error creating a hotplug callback\n");
        printf(libusb_error_name(rc));
        qDebug() << rc;
        libusb_exit(nullptr);
        return EXIT_FAILURE;
      }

//      while (count < 2) {
//        libusb_handle_events_completed(NULL, NULL);
//        nanosleep(&(struct timespec){0, 10000000UL}, NULL);
//      }

      libusb_hotplug_deregister_callback(nullptr, callback_handle);
      libusb_exit(nullptr);



    QApplication app(argc, argv);
       MainWindow w;

       w.show();
    return app.exec();
}
