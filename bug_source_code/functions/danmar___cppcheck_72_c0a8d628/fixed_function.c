            functionName == "free" ||
            functionName == "fclose" ||
            functionName == "realloc")
            continue;

        if (!CheckMemoryLeakInFunction::test_white_list(functionName, mSettings, mTokenizer->isCPP()))
            continue;

        const std::vector<const Token *> args = getArguments(tok);
        for (const Token* arg : args) {
            if (arg->isOp())
                continue;
            while (arg->astOperand1())
                arg = arg->astOperand1();
            if (getAllocationType(arg, 0) == No)
                continue;
            if (isReopenStandardStream(arg))
                continue;
            functionCallLeak(arg, arg->str(), functionName);
        }

    }
}