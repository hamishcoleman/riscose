
void swi_register_all(void)
{
  osmisc_swi_register();
  basictrans_swi_register();
  sharedclibrary_swi_register();
}

