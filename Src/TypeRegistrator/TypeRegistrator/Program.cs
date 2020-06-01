using System.IO;
using System.Collections.Generic;

namespace TypeRegistrator
{
    //TODO: clean file before any registration to ensure unused headers and types removed
    //TODO: exclude registrator exe from gitignore
    //TODO: run exe on project build prestep
    class Program
    {
        static void Main(string[] args)
        {
#region Prepare args
            //TODO: pass this via args
            bool newOutput = false;

            //string setMacro = "DECLARE_REGISTERED_TAG";
            //string getMacro = "REGISTERED_TAGS";

            string setMacro = "REGISTER_COMPONENT";
            string getMacro = "REGISTERED_COMPONENTS";

            string doc = "Documents"; // "OneDrive/Документы";

            string sourceDirectory = "C:/Users/Alex/" + doc + "/GodotProjects/Projects/Prototype/Src/Prototype";
            string outputFile = "C:/Users/Alex/" + doc + "/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/RegisteredTypes.h";
            string registerationMacroMacroDefinitionFile = "C:/Users/Alex/" + doc + "/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/ComponentsMeta.h";

            sourceDirectory.Replace('\\', '/');
            outputFile.Replace('\\', '/');
            registerationMacroMacroDefinitionFile.Replace('\\', '/');

            string[] fileExcludes = new string[] { outputFile, registerationMacroMacroDefinitionFile };
#endregion

#region Gather
            var headers = new HashSet<string>();
            var types = new HashSet<string>();

            new TypeGatherer().GatherRegisteredTypes(sourceDirectory, setMacro, outputFile, fileExcludes, headers, types);
#endregion

            if (types.Count > 0)
            {
                string output = new HeaderAssembler().GetHeaderSource(newOutput, outputFile, headers, types, getMacro);
                File.WriteAllText(outputFile, output);
            }
        }
    }
}
