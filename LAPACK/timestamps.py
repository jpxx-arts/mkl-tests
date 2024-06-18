import sys
import subprocess
import os

def LAPACK_exec(args, timestamps):
    try:
        comando = ['make', 'run', f'ARGS={args}']
        resultado = subprocess.run(comando, check=True, universal_newlines=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        # Supondo que os timestamps sejam a última linha do stdout
        timestamps.append(resultado.stdout.strip().split('\n')[-1])
    except subprocess.CalledProcessError as e:
        print(f"Erro ao executar comando: {comando}")
        print(e)
        print(e.stdout)
        print(e.stderr)

def run_experiments(matrix_type, specific_test, initial_size, final_size, interval, filename_prefix):
    precision_types = ['s', 'd', 'c', 'z']
    timestamps = {pt: [] for pt in precision_types}
    
    output_dir = f"./experiments/{filename_prefix}"
    os.makedirs(output_dir, exist_ok=True)
    
    for pt in precision_types:
        for num in range(initial_size, final_size, interval):
            arguments = f"{num} {num} {pt} {matrix_type} fis n 1 {specific_test}"
            LAPACK_exec(arguments, timestamps[pt])
        
        with open(f"{output_dir}/{pt}_{filename_prefix}_matrix.txt", 'w') as file:
            for exp in timestamps[pt]:
                file.write(exp + '\n')

if __name__ == "__main__":
    if len(sys.argv) != 6:
        print("Uso: script.py <matrix_type> <specific_op> <initial_size> <final_size> <interval>")
        sys.exit(1)

    initial_size = int(sys.argv[3])
    final_size = int(sys.argv[4])
    interval = int(sys.argv[5])
    matrix_type = sys.argv[1]
    specific_op = sys.argv[2]

    # General matrices
    if matrix_type == 'g' and specific_op == 'n':
        run_experiments('g', 'n', initial_size, final_size, interval, 'general')
    
    # Symmetric general matrices
    elif matrix_type == 'gs':
        run_experiments('gs', 'n', initial_size, final_size, interval, 'symmetric_general')
        
    # Triangular general matrices
    elif matrix_type == 'gt':
        run_experiments('gt', 'n', initial_size, final_size, interval, 'triangular_general')
    
    # Triangular general matrices
    elif matrix_type == 't' and specific_op == 'n':
        run_experiments('t', 'n', initial_size, final_size, interval, 'triangular_general')
    
    # Symmetric matrices
    elif matrix_type == 's':
        run_experiments('s', 'n', initial_size, final_size, interval, 'symmetric')
    
    # Triangular matrices
    elif matrix_type == 't':
        run_experiments('t', 'n', initial_size, final_size, interval, 'triangular')