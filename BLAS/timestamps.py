import subprocess

def BLAS_exec(args, timestamps):
    try:
        comando = ['make', 'run', f'ARGS={args}']
        
        resultado = subprocess.run(comando, check=True, text=True, capture_output=True)
        
        timestamps.append(resultado.stdout)
        
    except subprocess.CalledProcessError as e:
        print("Erro ao executar o comando:")
        print(e)
        print(e.stdout)
        print(e.stderr)

initial_size = 300
final_size = 6000
interval = 100
matrix_type = 's'

#general matrices
if matrix_type == 'g':
    timestamps_mm_s_g = []
    for num in range(initial_size, final_size, interval):
        arguments = f"{num} {num} {num} {num} mm s g n 1"
        BLAS_exec(arguments, timestamps_mm_s_g)

    timestamps_mm_d_g = []
    for num in range(initial_size, final_size, interval):
        arguments = f"{num} {num} {num} {num} mm d g n 1"
        BLAS_exec(arguments, timestamps_mm_d_g)

    timestamps_mm_c_g = []
    for num in range(initial_size, final_size, interval):
        arguments = f"{num} {num} {num} {num} mm c g n 1"
        BLAS_exec(arguments, timestamps_mm_c_g)

    timestamps_mm_z_g = []
    for num in range(initial_size, final_size, interval):
        arguments = f"{num} {num} {num} {num} mm z g n 1"
        BLAS_exec(arguments, timestamps_mm_z_g)

    with open("./experiments/general/float_general_matrix.txt", 'w') as file:
        for exp in timestamps_mm_s_g:
            file.write(exp)

    with open("./experiments/general/double_general_matrix.txt", 'w') as file:
        for exp in timestamps_mm_d_g:
            file.write(exp)

    with open("./experiments/general/complex_float_general_matrix.txt", 'w') as file:
        for exp in timestamps_mm_c_g:
            file.write(exp)

    with open("./experiments/general/complex_double_general_matrix.txt", 'w') as file:
        for exp in timestamps_mm_z_g:
            file.write(exp)

#sparse general matrices
elif matrix_type == 'ge':
    timestamps_mm_s_ge = []
    for num in range(initial_size, final_size, interval):
        arguments = f"{num} {num} {num} {num} mm s ge n 1"
        BLAS_exec(arguments, timestamps_mm_s_ge)

    timestamps_mm_d_ge = []
    for num in range(initial_size, final_size, interval):
        arguments = f"{num} {num} {num} {num} mm d ge n 1"
        BLAS_exec(arguments, timestamps_mm_d_ge)

    timestamps_mm_c_ge = []
    for num in range(initial_size, final_size, interval):
        arguments = f"{num} {num} {num} {num} mm c ge n 1"
        BLAS_exec(arguments, timestamps_mm_c_ge)

    timestamps_mm_z_ge = []
    for num in range(initial_size, final_size, interval):
        arguments = f"{num} {num} {num} {num} mm z ge n 1"
        BLAS_exec(arguments, timestamps_mm_z_ge)

    with open("./experiments/sparse general/float_sparse_general_matrix.txt", 'w') as file:
        for exp in timestamps_mm_s_ge:
            file.write(exp)

    with open("./experiments/sparse general/double_sparse_general_matrix.txt", 'w') as file:
        for exp in timestamps_mm_d_ge:
            file.write(exp)

    with open("./experiments/sparse general/complex_float_sparse_general_matrix.txt", 'w') as file:
        for exp in timestamps_mm_c_ge:
            file.write(exp)

    with open("./experiments/sparse general/complex_double_sparse_general_matrix.txt", 'w') as file:
        for exp in timestamps_mm_z_ge:
            file.write(exp)

#symmetric matrices
elif matrix_type == 's':
    timestamps_mm_s_s = []
    for num in range(initial_size, final_size, interval):
        arguments = f"{num} {num} {num} {num} mm s s n 1"
        BLAS_exec(arguments, timestamps_mm_s_s)

    timestamps_mm_d_s = []
    for num in range(initial_size, final_size, interval):
        arguments = f"{num} {num} {num} {num} mm d s n 1"
        BLAS_exec(arguments, timestamps_mm_d_s)

    timestamps_mm_c_s = []
    for num in range(initial_size, final_size, interval):
        arguments = f"{num} {num} {num} {num} mm c s n 1"
        BLAS_exec(arguments, timestamps_mm_c_s)

    timestamps_mm_z_s = []
    for num in range(initial_size, final_size, interval):
        arguments = f"{num} {num} {num} {num} mm z s n 1"
        BLAS_exec(arguments, timestamps_mm_z_s)

    with open("./experiments/symmetric/float_symmetric_matrix.txt", 'w') as file:
        for exp in timestamps_mm_s_s:
            file.write(exp)

    with open("./experiments/symmetric/double_symmetric_matrix.txt", 'w') as file:
        for exp in timestamps_mm_d_s:
            file.write(exp)

    with open("./experiments/symmetric/complex_float_symmetric_matrix.txt", 'w') as file:
        for exp in timestamps_mm_c_s:
            file.write(exp)

    with open("./experiments/symmetric/complex_double_symmetric_matrix.txt", 'w') as file:
        for exp in timestamps_mm_z_s:
            file.write(exp)