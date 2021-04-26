import sys
import os
import json

if len(sys.argv) <= 2:
    print("\nWrong arguments")
    print("Use: python3 test-deployer --source <target directory>\n")

    sys.exit(1)
else:
    if sys.argv[1] == '--source':
        # Directory path and list
        path = sys.argv[2]
        if path.startswith('/'):
            path = '.' + path
        elif not path.startswith('./'):
            path = './' + path
        file_dir = os.listdir(path)

    files = {
        "include": []
    }

    # Naming convention:
    #   test files end with '_test.c'
    #   and executables are '<target>_unit_test
    #   makefile with options 'make <target>' for each test file
    #   otherwise this script will have to be edited for each project
    for file in file_dir:
        if file.endswith('_test.c'):
            file_name = file.replace('.c', '')
            test_file = file_name.replace('_test', '_unit_test')
            file_path = path + test_file

            file_info = {
                "name": file_name,
                "test_name": test_file,
                "path": file_path
            }

            files['include'].append(file_info)

    # Convert the dictionary to JSON format
    json_target = json.dumps(files)

    with open(".github/workflows/test-list.json", "w") as json_file:
        json_file.write(json_target)
    json_file.close()

    print("JSON created successfully for folder " + sys.argv[2])
