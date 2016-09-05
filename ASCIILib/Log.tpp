template<typename T> void ascii::Log::Print(T message, bool newLine)
{
    if (Enabled())
    {
        // Put the message together in a stream
        stringstream messageStream;
        messageStream << message;
        if (newLine)
        {
            messageStream << endl;
        }

        // Output the message to the console
        cout << messageStream.str();

        // If an output filename is set, output to a file also
        if (!sOutputFilename.empty())
        {
            ofstream fileStream(sOutputFilename.c_str(), ios_base::app);

            fileStream << messageStream.str();
        }
    }
}

template<typename T> void ascii::Log::Error(T errorMessage)
{
    // Construct the message in one stream instead of calling Print() 5 times,
    // to save file open operations
    stringstream messageStream;
    messageStream << endl;
    messageStream << "*========================ERROR========================*" << endl;
    messageStream << errorMessage << endl;
    messageStream << "*========================ERROR========================*" << endl;
    messageStream << endl;

    Print(messageStream.str());
}
