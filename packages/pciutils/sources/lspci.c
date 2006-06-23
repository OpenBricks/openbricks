#include <stdio.h>
#include <sys/types.h>
#include <pci/pci.h>

static void print_device(struct pci_dev *pdev)
{
  printf("%04x: %04x:%04x (rev %d)\n", 
	    pci_read_word(pdev, PCI_CLASS_DEVICE),
	    pci_read_word(pdev, PCI_VENDOR_ID),
	    pci_read_word(pdev, PCI_DEVICE_ID),
	    pci_read_byte(pdev, PCI_REVISION_ID));
}

static void print_devices(struct pci_access *paccess)
{
  struct pci_dev *pdev;

  pci_scan_bus(paccess);
  for (pdev = paccess->devices; pdev; pdev = pdev->next)
  {
    print_device(pdev);
  }
}

static void blank_error(char *msg, ...) {
  return;
}

/* Main */

int
main(void)
{
  struct pci_access *paccess;

  paccess = pci_alloc();
  paccess->error = &blank_error;
  paccess->debug = &blank_error;
  paccess->warning = &blank_error;

  pci_init(paccess);
  print_devices(paccess);
  pci_cleanup(paccess);

  return 0;  
}
