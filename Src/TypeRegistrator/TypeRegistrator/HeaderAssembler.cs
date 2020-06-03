using System.Collections.Generic;
using System.Text;
using System.IO;

namespace TypeRegistrator
{
    class HeaderAssembler
    {
        public const string HEADER_TEMPLATE = "#pragma once" +
                                              "\r\n\r\n" +
                                              "//registered types" +
                                              "\r\n\r\n" +
                                              "//meta types declaration\r\n";

        private const int MAX_DEFINITION_LINE_LENGTH = 100;

        public string GetHeaderSource(string outputFile, HashSet<string> headers, HashSet<string> types, string getMacro)
        {
            var output = new StringBuilder(File.ReadAllText(outputFile));

            MergeHeaders(output, headers);

            ClearPreviouslyDefinedMacro(output, getMacro);
            output.Append("\r\n#define " + getMacro + " \\\r\n");
            output.Append(GetMacroDefinitionForTypes(types));

            return output.ToString();
        }

        private void MergeHeaders(StringBuilder output, HashSet<string> headers)
        {
            var pragma = "#pragma once\r\n\r\n";

            var outputStr = output.ToString();
            var concatedHeaders = new StringBuilder();
            foreach (var header in headers)
            {
                if (outputStr.IndexOf(header) == -1)
                    concatedHeaders.Append("#include \"" + header + "\"\r\n");
            }
            if (concatedHeaders.Length > 0)
                concatedHeaders.Append("\r\n");

            output.Insert(pragma.Length, concatedHeaders);
        }

        private void ClearPreviouslyDefinedMacro(StringBuilder output, string getMacro)
        {
            var outputStr = output.ToString();

            string macroDeclaration = "#define " + getMacro + " \\\r\n";
            int macroIndex = outputStr.IndexOf(macroDeclaration);
            if (macroIndex == -1)
                return;
            int macroEndIndex = macroIndex + macroDeclaration.Length;
            for (; macroEndIndex < outputStr.Length; macroEndIndex++)
            {
                if (outputStr[macroEndIndex] == '\n' && macroEndIndex > 1 && outputStr[macroEndIndex - 2] != '\\')
                    break;
            }

            if (macroIndex > 0)
                macroIndex -= 2;
            output.Remove(macroIndex, macroEndIndex - macroIndex - 1);
        }

        private string GetMacroDefinitionForTypes(HashSet<string> types)
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
                    macro.Append(", \\\r\n    " + currentType);
                    lineLengthCounter = currentType.Length + 8;
                }
                else
                {
                    macro.Append(", " + currentType);
                    lineLengthCounter += currentType.Length + 2;
                }
            }

            return macro.ToString();
        }
    }
}
