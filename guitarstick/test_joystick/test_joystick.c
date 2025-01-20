#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

int main(int argc, char *argv[])
{
  const char *device = "/dev/input/js0";
  struct js_event event;
  int fd;

  fd = open(device, O_RDONLY);

  if (fd < 0)
  {
    printf("Couldn't open %s\n", device);
    exit(1);
  }

  while (1)
  {
    int n = read(fd, &event, sizeof(event));

    if (n != sizeof(event))
    {
      printf("Error: n=%d sizeof(event)=%ld\n", n, sizeof(event));
      break;
    }

    switch (event.type)
    {
      case JS_EVENT_BUTTON:
        printf("Button event\n");
        break;
      case JS_EVENT_AXIS:
        printf("Axis event number=%d value=%d\n", event.number, event.value);
        break;
      default:
        printf("Other event %d\n", event.type);
        break;
    }
  }

  close(fd);

  return 0;
}

