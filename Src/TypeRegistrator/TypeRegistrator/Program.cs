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
            //TODO: pass this via args
            //string registerationMacro = "DECLARE_REGISTERED_TAG";
            //string getRegisteredMacro = "REGISTERED_TAGS";

            string registerationMacro = "REGISTER_COMPONENT";
            string getRegisteredMacro = "REGISTERED_COMPONENTS";

            string sourceDirectory = "C:/Users/Alex/Documents/GodotProjects/Projects/Prototype/Src/Prototype";
            string outputFile = "C:/Users/Alex/Documents/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/RegisteredTypes.h";
            string registerationMacroMacroDefinitionFile = "C:/Users/Alex/Documents/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/ComponentsMeta.h";

            sourceDirectory.Replace('\\', '/');
            outputFile.Replace('\\', '/');
            registerationMacroMacroDefinitionFile.Replace('\\', '/');

            string[] fileExcludes = new string[] { outputFile, registerationMacroMacroDefinitionFile };

            RegisterTypes(sourceDirectory, registerationMacro, getRegisteredMacro, outputFile, fileExcludes);
        }

        static bool Exclude(string file, string[] excludes)
        {
            foreach (var exclude in excludes)
            {
                if (file.Equals(exclude))
                    return true;
            }

            return false;
        }

        static void RegisterTypes(string sourceDirectory, string registerationMacro, string getRegisteredMacro, string outputFile, string[] excludes)
        {
            string[] allfiles = Directory.GetFiles(sourceDirectory, "*.h", SearchOption.AllDirectories);
            HashSet<string> headers = new HashSet<string>();
            HashSet<string> types = new HashSet<string>();

            for (int i = 0; i < allfiles.Length; i++)
            {
                string file = allfiles[i].Replace('\\', '/');
                if (Exclude(file, excludes))
                    continue;

                GatherRegisteredTypes(file, registerationMacro, types, headers);
            }

            if (types.Count > 0)
            {
                //TODO: make it via FileStream and fix not consistent line endings
                WriteHeaders(outputFile, headers);
                string macroDefinition = GetMacroDefinitionForTypes(types);
                WriteRegisteredTypes(outputFile, macroDefinition, getRegisteredMacro);
            }
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

        static int GetTypeEndIndex(string src, int typeStartIndex)
        {
            char[] closingChars = new char[] { ')', ',' };
            int typeEndIndex = -1;
            foreach (var closingChar in closingChars)
            {
                int closingCharIndex = src.IndexOf(closingChar, typeStartIndex);
                if (typeEndIndex < 0)
                    typeEndIndex = closingCharIndex;
                else if (closingCharIndex > 0 && closingCharIndex < typeEndIndex)
                    typeEndIndex = closingCharIndex;
            }

            if (typeEndIndex < 0)
            {
                Console.WriteLine("Error in source code. No closing braket");
                Environment.Exit(1);
            }

            return typeEndIndex;
        }

        static void GatherRegisteredTypes(string file, string tag, HashSet<string> types, HashSet<string> headers)
        {
            var src = File.ReadAllText(file);

            for (int tagIndex = 0; ; tagIndex += tag.Length)
            {
                tagIndex = src.IndexOf(tag, tagIndex);
                if (tagIndex == -1)
                    return;

                headers.Add(file);

                int typeStartIndex = tagIndex + tag.Length + 1;
                int typeEndIndex = GetTypeEndIndex(src, typeStartIndex);

                string type = src.Substring(typeStartIndex, typeEndIndex - typeStartIndex);
                if (!types.Add(type))
                {
                    Console.WriteLine("Error in source code. Type registered more than once");
                    Environment.Exit(2);
                }
            }
        }

        static void WriteRegisteredTypes(string path, string macroDefinition, string macro)
        {
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

            int endLineIndex = src.IndexOf('\n', insertIndex);
            if (endLineIndex < 0)
                srcBuilder.Remove(insertIndex, src.Length - insertIndex);
            else
            {
                if (src[endLineIndex - 1] == '\\')
                    ClearPreviouslyDefined(srcBuilder, endLineIndex + 1);
                srcBuilder.Remove(insertIndex, endLineIndex - insertIndex);
            }
        }

        //TODO: fix appending new line on every WriteHeaders
        static void WriteHeaders(string path, HashSet<string> headers)
        {
            StringBuilder srcBuilder = new StringBuilder(File.ReadAllText(path));
            
            int insertIndex = GetHeaderInsertIndex(srcBuilder);

            var headersEnumerator = headers.GetEnumerator();
            while (headersEnumerator.MoveNext())
            {
                string include = "#include \"" + headersEnumerator.Current + "\"";
                if (srcBuilder.ToString().IndexOf(include) < 0)
                    srcBuilder.Insert(insertIndex, include + '\n');
            }

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

            if (srcStr.Length == insertIndex)
                srcBuilder.Insert(insertIndex, '\n');
            srcStr = srcBuilder.ToString();

            if (srcStr[insertIndex] != '\n')
                srcBuilder.Insert(insertIndex, '\n');
            srcBuilder.Insert(insertIndex, '\n');

            insertIndex += 2;

            return insertIndex;
        }
    }
}
