Building your own kernel and installing it in the Chromebook isn't
that hard, but there are a couple of extra steps needed.

The kernel resides in the boot partition, and it has to be signed for
the verified boot firmware to load and boot it. In developer mode,
it can be signed with any key though, including the default developer
keys provided on the Chrome OS filesystem.

To make life easier on Ubuntu, I copied over some of the tools from
Chrome OS. One of them needs libssl 0.9.8, so you need to install that:

# apt-get install libssl0.9.8

Then, the vbutil_kernel tool is used to pack and sign the kernel image
that is written to disk. You'll also want to install the keys, etc:

# mount -o ro /dev/mmcblk0p3 /mnt
# cp /mnt/usr/bin/vbutil_kernel /usr/bin
# rsync -a /mnt/usr/share/vboot /usr/share

There's also another tool that is convenient to have, dump_kernel_config.
It actually dumps the command line, not the kernel config, from a packed
kernel image. You can also use vbutil_kernel to do it, but this tool is
more practical for scripting. So, also:

# cp /mnt/usr/bin/dump_kernel_config /usr/bin

Also, if you haven't already copied over cgpt, do so now.

# cp /mnt/usr/bin/cgpt /usr/bin

And then, of course:

# umount /mnt

Now you have the tools you need on the filesystem. Just to make sure they
work, try verifying the current kernel on the disk. So:

# vbutil_kernel --verify /dev/mmcblk1p1

Now, configuring and building a kernel is something that I will mostly
skip over here. People like Marcin Juszkiewicz and others who clean up
my rough instructions and package things up for the distro will sort
most of that out. Just a couple of quick pointers:

The sources for the Chrome OS kernel are at
https://git.chromium.org/git/chromiumos/third_party/kernel.git. The
current active branch there is chromeos-3.4. To find the branch that
corresponds to the release that your Chromebook runs, see the release-R*
branches.

To generate the kernel config that we build with, besides getting it
from /boot on the Chrome OS filesystem, you can (in the source tree) run:

$ chromeos/scripts/prepareconfig chromeos-exynos5

It will create the .config file for you.

Then just use the regular distro build tools (make-kpkg), or just make
+ make_install. If you want/need to rebuild the kernel, you probably
know how to do these things already. If not, just use the kernel
copied over from Chrome OS.

The one thing I've done differently than the default build tools
is that I tend to also do a "make dtbs" and copy the contents from
arch/arm/boot/dts/*.dtb into /boot. The script below uses those paths
so revise accordingly if you don't do the same.

So, the kernel format used on ARM Chrom{e,ium} OS is a combined format
called FIT Image. It's produced by the mkimage u-boot tool, which is
provided by the u-boot-tools package on Ubuntu:

# apt-get install u-boot-tools

Then you need a source file (kernel.its). Mine contains:

/dts-v1/;

/ {
    description = "Chrome OS kernel image with one or more FDT blobs";
    #address-cells = <1>;
    images {
        kernel@1{
   description = "kernel";
            data = /incbin/("/boot/zImage");
            type = "kernel_noload";
            arch = "arm";
            os = "linux";
            compression = "none";
            load = <0>;
            entry = <0>;
        };
        fdt@1{
            description = "exynos5250-snow.dtb";
            data = /incbin/("/boot/exynos5250-snow.dtb");
            type = "flat_dt";
            arch = "arm";
            compression = "none";
            hash@1{
                algo = "sha1";
            };
        };
    };
    configurations {
        default = "conf@1";
        conf@1{
            kernel = "kernel@1";
            fdt = "fdt@1";
        };
    };
};


Then, to create the binary kernel image, you do:

mkimage -f kernel.its kernel.itb

That creates kernel.itb as the binary image. Finally, you need to sign
this with the verified boot tools. You also need to specify the kernel
command line as part of this. The command to do so is:

$ echo "console=tty1 debug root=/dev/mmcblk1p3 rw rootwait" > config.txt

$ vbutil_kernel --pack /tmp/newkernel \
      --keyblock /usr/share/vboot/devkeys/kernel.keyblock \
      --version 1 \
      --signprivate /usr/share/vboot/devkeys/kernel_data_key.vbprivk \
      --config config.txt --vmlinuz kernel.itb --arch arm

Then you write out the newly generated kernel to disk. I use the second
kernel partition for my test kernels:

# dd if=/tmp/newkernel of=/dev/mmcblk1p2

Then you need to mark the second kernel partition bootable, and you need
to mark it as higher priority than the first kernel partition. Here is
where verified boot has a feature that is very convenient when you are
testing new kernels:

If you mark the kernel partition as higher priority, but don't set the
"successful" bit in the partition table, it will try to boot it as
many times as the "tries" field specifies, decrementing it each time.

This means that if I want to try booting a kernel just once, and if
that doesn't work, boot back to the known good kernel, I will set it to
something like:

# cgpt add -i 2 -P 15 -S 0 -T 1 /dev/mmcblk1

In other words: For partition index 2 (mmcblk1p2), set priority to 15,
successful to 0 and tries to 1. Since the first partition has priority
10, it means this will be tried first, as long as the tries counter is >0.

To see what your settings are, use:

# cgpt show -i 2 /dev/mmcblk1

(and compare with -i 1).

That mostly wraps up most of what I was planning on writing up so far.

Based on my descriptions here, it seems like some of the distro guys
have gotten enough going that they are mostly self sufficient. I'm of
course happy to help answer questions, and I want to make sure that the
platform is convenient to work on once we have upstreamed most of our
kernel changes over the upcoming months, but beyond that I'll leave most
of the polish and packaging up to others.
