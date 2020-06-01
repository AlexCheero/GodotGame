using System.Collections.Generic;
using System.Text;
using System.IO;

namespace TypeRegistrator
{
    class HeaderAssembler
    {
        const int MAX_DEFINITION_LINE_LENGTH = 100;

        public string GetHeaderSource(bool newOutput, string outputFile, HashSet<string> headers, HashSet<string> types, string getMacro)
        {
            if (newOutput)
                return AssembleOutput(headers, types, getMacro);
            else
                return MergeWithExistingOutput(outputFile, headers, types, getMacro);
        }

        private string AssembleOutput(HashSet<string> headers, HashSet<string> types, string getMacro)
        {
            var output = new StringBuilder();
            output.Append("#pragma once\r\n\n");

            foreach (var header in headers)
                output.Append("#include \"" + header + "\"\r\n");
            output.Append("\r\n");

            output.Append("#define " + getMacro + " \\\r\n");
            output.Append(GetMacroDefinitionForTypes(types));

            return output.ToString();
        }

        private string MergeWithExistingOutput(string outputFile, HashSet<string> headers, HashSet<string> types, string getMacro)
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
            var pragma = "#pragma once\r\n\n";

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

            string macroDeclaration = "#define " + getMacro + " \\\r\n"; //TODO: don't forget about \r in other places
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
            output.Remove(macroIndex, macroEndIndex - macroIndex);
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
