#!/usr/sbin/dtrace -s
#pragma D option quiet

/* This dtrace script echoes out all uses of the DKIOCDVDSETSPEED
 * ioctl by the traced process. Note that lots of apps which access
 * DVDs use PT_DENY_ATTACH to disable debugging/tracing - you may have
 * to work around using something like
 *
 * http://landonf.bikemonkey.org/code/macosx/Leopard_PT_DENY_ATTACH.20080122.html
 */


syscall::ioctl:entry
/arg1 == 0x80026483/
{
	raw = (uint16_t *)copyin(arg2, 2);

	self->start = timestamp;

	self->fd = arg0;
	self->request = "DKIOCDVDSETSPEED";
	self->data = (int) raw[0];
}

syscall::ioctl:return
/self->start/
{
	printf("%5d\t", pid);
	printf("%s(0x%X,  %s, in_speed = %d)\t\t = %d %s%d\n",
		probefunc,
		self->fd,
		self->request,
		self->data,
		(int) arg0,
		errno == 0 ? "" : "Err#",
		(int) errno);

	self->start = 0;
}

