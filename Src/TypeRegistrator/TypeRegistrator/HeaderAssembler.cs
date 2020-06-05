﻿using System.Collections.Generic;
using System.Text;
using System.IO;

namespace TypeRegistrator
{
    class HeaderAssembler
    {
        private const int MAX_DEFINITION_LINE_LENGTH = 100;
        private const string PRAGMA = "#pragma once\r\n\r\n";
        private const string REG_TYPES_SECTION_TAG = "//registered types\r\n";
        private const string META_TYPES_SECTION_TAG = "//meta types declaration\r\n";

        public const string HEADER_TEMPLATE = PRAGMA + REG_TYPES_SECTION_TAG + "\r\n\r\n" + META_TYPES_SECTION_TAG;

        public string GetHeaderSource(string outputFile, HashSet<string> headers, Dictionary<string, List<string>> types,
            string getMacro, bool gatherWithFields, string declareMetaMacro = "")
        {
            var output = new StringBuilder(File.ReadAllText(outputFile));
            
            MergeHeaders(output, headers);
            ClearPreviouslyDefinedMacro(output, getMacro);

            var outputStr = output.ToString();

            int regTypesSectionIndex = outputStr.IndexOf(REG_TYPES_SECTION_TAG) + REG_TYPES_SECTION_TAG.Length;

            //TODO: move #define and other literals to separate file with string constants
            string macroDeclaration = "\r\n#define " + getMacro + " \\\r\n";
            output.Insert(regTypesSectionIndex, macroDeclaration);
            output.Insert(regTypesSectionIndex + macroDeclaration.Length, GetMacroDefinitionForTypes(types));

            if (declareMetaMacro.Length > 0)
            {
                outputStr = output.ToString();
                int metaTypesSectionIndex = outputStr.IndexOf(META_TYPES_SECTION_TAG) + META_TYPES_SECTION_TAG.Length;
                output.Insert(metaTypesSectionIndex, "\r\n");
                output.Insert(metaTypesSectionIndex + 2, GetMetaTypeDeclarations(types, declareMetaMacro, gatherWithFields));
            }

            return output.ToString();
        }

        private void MergeHeaders(StringBuilder output, HashSet<string> headers)
        {
            var outputStr = output.ToString();
            var concatedHeaders = new StringBuilder();
            foreach (var header in headers)
            {
                if (outputStr.IndexOf(header) == -1)
                    concatedHeaders.Append("#include \"" + header + "\"\r\n");
            }
            if (concatedHeaders.Length > 0)
                concatedHeaders.Append("\r\n");

            output.Insert(PRAGMA.Length, concatedHeaders);
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

        private string GetMacroDefinitionForTypes(Dictionary<string, List<string>> types)
        {
            var macro = new StringBuilder();

            var typeSetEnumerator = types.Keys.GetEnumerator();
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

        private string GetMetaTypeDeclarations(Dictionary<string, List<string>> types, string declarationMacro, bool gatherWithFields)
        {
            var declarations = new StringBuilder();

            foreach (var type in types)
            {
                if (gatherWithFields)
                    declarations.Append(declarationMacro + '(' + type.Key + ", " + GetFieldsEnumeration(type.Value) + ");\r\n");
                else
                    declarations.Append(declarationMacro + '(' + type.Key + ");\r\n");
            }

            return declarations.ToString();
        }

        private string GetFieldsEnumeration(List<string> fields)
        {
            return '\"' + string.Join("\", \"", fields) + '\"';
        }
    }
}
