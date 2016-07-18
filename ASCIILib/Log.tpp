template<typename T> void ascii::Log::Print(T message, bool newLine)
{
    if (Enabled())
    {
        cout << message;
        if (newLine)
        {
            cout << endl;
        }
    }
}

template<typename T> void ascii::Log::Error(T errorMessage)
{
    Print("*========================ERROR========================*");
    Print(errorMessage);
    Print("*========================ERROR========================*");
}
