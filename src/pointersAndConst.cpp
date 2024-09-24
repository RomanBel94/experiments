int main()
{
    int var{ 0 };
    const int* ciptr = &var;
    int const* icptr = &var;
    int* const iscptr = &var;
    const int* const cptr = var;

    ciptr++;
    (*ciptr)++;

    icptr++;
    (*icptr)++;

    iscptr++;
    (*iscptr)++;

    cptr++;
    (*cptr)++;
}

