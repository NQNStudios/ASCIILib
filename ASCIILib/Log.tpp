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
            FILE* file = fopen(sOutputFilename.c_str(), "a");

			if (file == NULL)
			{
                cout << "Error! Could not write log output to file." << endl;
			}
			else
			{
                fprintf(file, messageStream.str().c_str());
                fclose(file);
			}
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
