#define LIST \
    X(basictrans) \
    X(osmisc) \
    X(sharedclibrary) \

#define X(area) void area ## _swi_register(void);
LIST
#undef X

void swi_register_all(void)
{
#define X(area) area ## _swi_register();
    LIST
#undef X

    return;
}
