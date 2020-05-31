using System;
using System.IO;
using System.Collections.Generic;
using System.Text;

namespace TypeRegistrator
{
    //TODO: exclude registrator exe from gitignore
    //TODO: run exe on project build prestep
    class Program
    {
        static void Main(string[] args)
        {
#region Prepare args
            //TODO: pass this via args
            string registerationMacro = "DECLARE_REGISTERED_TAG";
            string getRegisteredMacro = "REGISTERED_TAGS";

            //string registerationMacro = "REGISTER_COMPONENT";
            //string getRegisteredMacro = "REGISTERED_COMPONENTS";

            string sourceDirectory = "C:/Users/Alex/OneDrive/Документы/GodotProjects/Projects/Prototype/Src/Prototype";
            string outputFile = "C:/Users/Alex/OneDrive/Документы/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/RegisteredTypes.h";
            string registerationMacroMacroDefinitionFile = "C:/Users/Alex/OneDrive/Документы/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/ComponentsMeta.h";

            sourceDirectory.Replace('\\', '/');
            outputFile.Replace('\\', '/');
            registerationMacroMacroDefinitionFile.Replace('\\', '/');

            string[] fileExcludes = new string[] { outputFile, registerationMacroMacroDefinitionFile };
#endregion

#region Gather
            var gatherer = new TypeGatherer();

            var headers = new HashSet<string>();
            var types = new HashSet<string>();

            gatherer.GatherRegisteredTypes(sourceDirectory, registerationMacro, outputFile, fileExcludes, headers, types);

            Console.WriteLine("headers: ");
            foreach (var header in headers)
                Console.WriteLine(header);
            Console.WriteLine();
            Console.WriteLine("types: ");
            foreach (var type in types)
                Console.WriteLine(type);
#endregion

            return;

            if (types.Count > 0)
                RegisterTypes(getRegisteredMacro, outputFile, headers, types);
        }

        static void RegisterTypes(string getRegisteredMacro, string outputFile, HashSet<string> headers, HashSet<string> types)
        {
            //TODO: fix inconsistent line endings
            //TODO: make it via FileStream or try to use File.WriteAllText, File.ReadAllText and StringBuilder only once
            WriteHeaders(outputFile, headers);
            //TODO: fix adding empty lines on re adding types. Probably this is because of StringBuilder.Remove
            WriteRegisteredTypes(outputFile, types, getRegisteredMacro);
        }

        const int MAX_DEFINITION_LINE_LENGTH = 100;
        static string GetMacroDefinitionForTypes(HashSet<string> types)
        {
            StringBuilder macro = new StringBuilder();

            var typeSetEnumerator = types.GetEnumerator();
            typeSetEnumerator.MoveNext();
            
            string currentType = typeSetEnumerator.Current;
            macro.Append("    " + currentType);

            int lineLengthCounter = currentType.Length;

            while (typeSetEnumerator.MoveNext())
            {
                currentType = typeSetEnumerator.Current;

                if (lineLengthCounter + currentType.Length + 2 > MAX_DEFINITION_LINE_LENGTH)
                {
                    macro.Append(", \\\n    " + currentType);
                    lineLengthCounter = currentType.Length + 8;
                }
                else
                {
                    macro.Append(", " + currentType);
                    lineLengthCounter += currentType.Length + 2;
                }
            }

            macro.Insert(0, "\\\n");

            return macro.ToString();
        }

        static void WriteRegisteredTypes(string path, HashSet<string> types, string macro)
        {
            string macroDefinition = GetMacroDefinitionForTypes(types);
            string src = File.ReadAllText(path);
            StringBuilder srcBuilder = new StringBuilder(src);

            string macroDefine = "#define " + macro + " ";
            int defineIndex = src.IndexOf(macroDefine);

            if (defineIndex > 0)
            {
                int insertIndex = defineIndex + macroDefine.Length;
                ClearPreviouslyDefined(srcBuilder, insertIndex);
                srcBuilder.Insert(insertIndex, macroDefinition);
            }
            else
            {
                srcBuilder.Append("\n" + macroDefine);
                srcBuilder.Append(macroDefinition);
            }

            File.WriteAllText(path, srcBuilder.ToString());
        }

        static void ClearPreviouslyDefined(StringBuilder srcBuilder, int insertIndex)
        {
            string src = srcBuilder.ToString();

            //TODO: clear is broken
            int endLineIndex = src.IndexOf('\n', insertIndex);
            if (endLineIndex < 0)
            {
                Console.WriteLine("clear last");
                srcBuilder.Remove(insertIndex, src.Length - insertIndex);
            }
            else
            {
                Console.WriteLine("clear string");
                if (src[endLineIndex - 1] == '\\')
                    ClearPreviouslyDefined(srcBuilder, endLineIndex + 1);
                srcBuilder.Remove(insertIndex, endLineIndex - insertIndex);
            }
        }

        static string GetHeadersString(HashSet<string> headers, string srcStr)
        {
            StringBuilder headersStr = new StringBuilder();
            foreach (var header in headers)
            {
                if (srcStr.IndexOf(header) < 0)
                    headersStr.Append("#include \"" + header + "\"\n");
            }

            return headersStr.ToString();
        }

        static void WriteHeaders(string path, HashSet<string> headers)
        {
            StringBuilder srcBuilder = new StringBuilder(File.ReadAllText(path));
            string srcStr = srcBuilder.ToString();

            string headersStr = GetHeadersString(headers, srcStr);
            if (headersStr.Length == 0)
                return;

            int insertIndex = GetHeaderInsertIndex(srcBuilder);
            if (srcStr.Length < insertIndex + 1)
                srcBuilder.Append("\n\n"); //nothing after pragma
            else if (srcStr[insertIndex] != '\n')
                srcBuilder.Insert(insertIndex, "\n\n"); //no new line after pragma
            else if (srcStr.Length < insertIndex + 2 || srcStr[insertIndex + 1] != '\n')
                srcBuilder.Insert(insertIndex, '\n'); //no empty line after pragma

            insertIndex += 2;
            
            srcBuilder.Insert(insertIndex, headersStr);
            int lastHeadersIndex = insertIndex + headersStr.Length;
            if (srcBuilder.ToString()[lastHeadersIndex] != '\n')
                srcBuilder.Insert(lastHeadersIndex, '\n');

            File.WriteAllText(path, srcBuilder.ToString());
        }

        static int GetHeaderInsertIndex(StringBuilder srcBuilder)
        {
            string pragma = "#pragma once";
            string srcStr = srcBuilder.ToString();
            int insertIndex = srcStr.IndexOf(pragma);

            if (insertIndex < 0)
            {
                Console.WriteLine("Error in source code. No #pragma once directive");
                Environment.Exit(3);
            }

            insertIndex += pragma.Length;
            if (srcStr.Length < insertIndex)
            {
                Console.WriteLine("Something went horribly wrong!");
                Environment.Exit(4);
            }

            return insertIndex;
        }
    }
}
