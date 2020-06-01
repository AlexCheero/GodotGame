using System.IO;
using System.Collections.Generic;
using System;

namespace TypeRegistrator
{
    //TODO: clean file before any registration to ensure unused headers and types removed
    //TODO: exclude registrator exe from gitignore
    //TODO: run exe on project build prestep
    class Program
    {
        static void Main(string[] args)
        {
            bool newOutput = bool.Parse(args[0]);
            
            string setMacro = args[1];//"DECLARE_REGISTERED_TAG";
            string getMacro = args[2];//"REGISTERED_TAGS";
            
            //string doc = /*"Documents"; //*/ "OneDrive/Документы";
            
            string sourceDirectory = args[3];//"C:/Users/Alex/" + doc + "/GodotProjects/Projects/Prototype/Src/Prototype";
            string outputFile = args[4];//"C:/Users/Alex/" + doc + "/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/RegisteredTypes.h";
            string registerationMacroMacroDefinitionFile = args[5];//"C:/Users/Alex/" + doc + "/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/ComponentsMeta.h";

            sourceDirectory = sourceDirectory.Replace('\\', '/');
            outputFile = outputFile.Replace('\\', '/');
            registerationMacroMacroDefinitionFile = registerationMacroMacroDefinitionFile.Replace('\\', '/');

            string[] fileExcludes = new string[] { outputFile, registerationMacroMacroDefinitionFile };

#region Prepare args
            //TODO: pass this via args
            //bool newOutput = false;
            //
            ////string setMacro = "DECLARE_REGISTERED_TAG";
            ////string getMacro = "REGISTERED_TAGS";
            //
            //string setMacro = "REGISTER_COMPONENT";
            //string getMacro = "REGISTERED_COMPONENTS";
            //
            //string doc = /*"Documents"; //*/ "OneDrive/Документы";
            //
            //string sourceDirectory = "C:/Users/Alex/" + doc + "/GodotProjects/Projects/Prototype/Src/Prototype";
            //string outputFile = "C:/Users/Alex/" + doc + "/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/RegisteredTypes.h";
            //string registerationMacroMacroDefinitionFile = "C:/Users/Alex/" + doc + "/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/ComponentsMeta.h";
            //
            //sourceDirectory.Replace('\\', '/');
            //outputFile.Replace('\\', '/');
            //registerationMacroMacroDefinitionFile.Replace('\\', '/');
            //
            //string[] fileExcludes = new string[] { outputFile, registerationMacroMacroDefinitionFile };
#endregion

            var headers = new HashSet<string>();
            var types = new HashSet<string>();

            new TypeGatherer().GatherRegisteredTypes(sourceDirectory, setMacro, outputFile, fileExcludes, headers, types);

            if (types.Count > 0)
            {
                string output = new HeaderAssembler().GetHeaderSource(newOutput, outputFile, headers, types, getMacro);
                File.WriteAllText(outputFile, output);
            }
        }
    }
}
