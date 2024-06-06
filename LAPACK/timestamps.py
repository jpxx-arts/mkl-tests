import subprocess

def LAPACK_exec(args, timestamps):
    try:
        comando = ['make', 'run', f'ARGS={args}']
        resultado = subprocess.run(comando, check=True, text=True, capture_output=True)
        timestamps.append(resultado.stdout)
    except subprocess.CalledProcessError as e:
        print(e)
        print(e.stdout)
        print(e.stderr)

def run_experiments(matrix_type, specific_test, initial_size, final_size, interval, filename_prefix):
    precision_types = ['s', 'd', 'c', 'z']
    timestamps = {pt: [] for pt in precision_types}

    for pt in precision_types:
        for num in range(initial_size, final_size, interval):
            arguments = f"{num} {num} {pt} {matrix_type} fis n 1 {specific_test}"
            LAPACK_exec(arguments, timestamps[pt])

        with open(f"./experiments/{filename_prefix}/{pt}_{filename_prefix}_matrix.txt", 'w') as file:
            for exp in timestamps[pt]:
                file.write(exp)

initial_size = 300
final_size = 600
interval = 100
matrix_type = 's'
specific_test = 'n'

# General matrices
if matrix_type == 'g':
    run_experiments('g', specific_test, initial_size, final_size, interval, 'general')

# Symmetric general matrices
elif matrix_type == 'gs':
    run_experiments('gs', specific_test, initial_size, final_size, interval, 'symmetric_general')

# Triangular general matrices
elif matrix_type == 'gt':
    run_experiments('t', specific_test, initial_size, final_size, interval, 'triangular_general')

# Symmetric matrices
elif matrix_type == 's':
    run_experiments('s', specific_test, initial_size, final_size, interval, 'symmetric')

# Triangular matrices
elif matrix_type == 't':
    run_experiments('t', specific_test, initial_size, final_size, interval, 'triangular')

# Specific transposition operations - manual
elif specific_test == 't':
    run_experiments('g', 't', initial_size, final_size, interval, 'manual_trans')

# Specific transposition operations - auto
elif specific_test == 'T':
    run_experiments('g', 'T', initial_size, final_size, interval, 'auto_trans')