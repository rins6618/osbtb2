import os

def add_header_guards(file_path):
    file_name = os.path.basename(file_path)
    guard = file_name.replace('.', '_').replace(' ', '_').upper()

    with open(file_path, 'r') as file:
        content = file.read()

    if f'#ifndef {guard}' in content and f'#define {guard}' in content:
        print(f"Header guards already exist in {file_path}")
        return

    new_content = f"#ifndef {guard}\n#define {guard}\n\n{content}\n\n#endif // {guard}\n"

    with open(file_path, 'w') as file:
        file.write(new_content)

    print(f"Added header guards to {file_path}")

def process_directory(directory):
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith('.h'):
                file_path = os.path.join(root, file)
                add_header_guards(file_path)

if __name__ == "__main__":
    current_directory = os.getcwd()
    process_directory(current_directory)