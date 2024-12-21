import re
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

img_path: str = '.\\images\\'
tab_path: str = '.\\tables\\'
res_pathes: dict = {
    'GMP': '.\\gmp_result_',
    'NTL': '.\\ntl_result_'
}
res_tests: dict = {
    1000: '1k.txt',
    10000: '10k.txt'
}

class data_struct:
    def __init__(self, lib: str, size: int, data: pd.DataFrame):
        self.name: str = lib
        self.size: int = size
        self.data: pd.DataFrame = data
    
    def plot_operations(self, exclude_types: list[str] = None, exclude_operations: list[str] = None, time_unit = 's'):
        df_pivot = self.data.copy()
        if exclude_types is not None:
            df_pivot = df_pivot.drop(columns=[ty for ty in exclude_types if ty in df_pivot.columns])
        if exclude_operations is not None:
            df_pivot = df_pivot.drop(index=[op for op in exclude_operations if op in df_pivot.index])
    
        if time_unit == 'ms':
            df_pivot = df_pivot * 1000  
            ylabel = 'Time (ms)'
        elif time_unit == 'us':
            df_pivot = df_pivot * 1_000_000  
            ylabel = 'Time (µs)'
        else:
            ylabel = 'Time (s)'  

        title = f"{self.name} ({self.size}) Operations Time by Type"
        ax = df_pivot.plot(kind='bar', figsize=(10, 6))
        ax.set_title(title)
        ax.set_ylabel(ylabel)
        ax.set_xlabel("Operation")
        ax.legend(title="Type")
        ax.grid(True, which='both', axis='both', linestyle='--', linewidth=0.5)

        plt.tight_layout()
        plt.savefig(img_path + title.replace(' ', '_') + 
                    str(len(exclude_types) if exclude_types is not None else 0) +
                    str(len(exclude_operations) if exclude_operations is not None else 0))
        

    def plot_time_vs_bits(self, exclude_types: list[str] = None, exclude_operations: list[str] = None, time_unit = 's'):
        plt.figure(figsize=(10, 6))

        df_pivot = self.data.drop(columns=['float'])
        if exclude_types is not None:
            df_pivot = df_pivot.drop(columns=[ty for ty in exclude_types if ty in df_pivot.columns])
        if exclude_operations is not None:
            df_pivot = df_pivot.drop(index=[op for op in exclude_operations if op in df_pivot.index])
    
        if time_unit == 'ms':
            df_pivot = df_pivot * 1000  
            ylabel = 'Time (ms)'
        elif time_unit == 'us':
            df_pivot = df_pivot * 1_000_000  
            ylabel = 'Time (µs)'
        else:
            ylabel = 'Time (s)' 

        title = f"{self.name} ({self.size}) Execution Time vs Type"
        for operation in df_pivot.index:
            operation_data = df_pivot.loc[operation]
            plt.plot(operation_data.index, operation_data.values, marker='o', label=operation)

        plt.legend(title="Operations")
        plt.grid(True, which='both', axis='both', linestyle='--', linewidth=0.5)
        plt.title(title)
        plt.xlabel("Type")
        plt.ylabel(ylabel)
        plt.yscale('log')
        plt.tight_layout()
        plt.savefig(img_path + title.replace(' ', '_') + 
                    str(len(exclude_types) if exclude_types is not None else 0) +
                    str(len(exclude_operations) if exclude_operations is not None else 0))


def parse_results(lib: str, file_path: str) -> data_struct:
    with open(file_path, 'r') as file:
        content = file.read()

    test_pattern = re.compile(r"TEST:\s*(\w+)")
    size_pattern = re.compile(r"SIZE:\s*(\d+)")
    bits_pattern = re.compile(r"BITS:\s*(\d+)")
    results_pattern = re.compile(r"RESULTS:\s*\{(.*?)\}", re.DOTALL)

    tests = test_pattern.findall(content)
    sizes = size_pattern.findall(content)
    bits = bits_pattern.findall(content)
    results_blocks = results_pattern.findall(content)

    operations_data = []
    for i, block in enumerate(results_blocks):
        operations = re.findall(r"(\w+):\s*([\d.]+)s", block)
        for operation, time in operations:
            operations_data.append({
                "Type": 'int_' + bits[i] if i < len(bits) else 'float',
                "Operation": operation,
                "Time_s": float(time)
            })

    df_operations = pd.DataFrame(operations_data)
    df_pivot = df_operations.pivot(index='Operation', columns='Type', values='Time_s')
    df_pivot = df_pivot[['float', 'int_128', 'int_256', 'int_512', 'int_1024']]

    return data_struct(lib, sizes[0], df_pivot)


def plot_comparsion(dfs: list[pd.DataFrame], names: list[str], size: int, exclude_types: list[str] = None, exclude_operations: list[str] = None, time_unit = 's'):
    if len(dfs) != len(names):
        raise ValueError("Size of 'dfs', 'names' mismatch!")
    
    for i, df_pivot in enumerate(dfs):
        if exclude_types is not None:
            df_pivot = df_pivot.drop(columns=[ty for ty in exclude_types if ty in df_pivot.columns])
        if exclude_operations is not None:
            df_pivot = df_pivot.drop(index=[op for op in exclude_operations if op in df_pivot.index])
        dfs[i] = df_pivot
    
        if time_unit == 'ms':
            df_pivot = df_pivot * 1000  
            ylabel = 'Time (ms)'
        elif time_unit == 'us':
            df_pivot = df_pivot * 1_000_000  
            ylabel = 'Time (µs)'
        else:
            ylabel = 'Time (s)' 
    
    all_data = []
    for table, library in zip(dfs, names):
        table['library'] = library
        table.reset_index(inplace=True)
        all_data.append(table)
    
    df_pivot = pd.concat(all_data, ignore_index=True)

    operations = df_pivot['Operation'].unique()
    num_operations = len(operations)
    bar_width = 0.15
    index = np.arange(num_operations)
    colors = plt.get_cmap('tab10', len(names))

    fig, ax = plt.subplots(figsize=(12, 7))
    
    for i, col in enumerate(df_pivot.columns[1:-2]):
        for j, library in enumerate(names):
            library_data = df_pivot[df_pivot['library'] == library]

            alpha = 0.7 - (j * 0.1)
            ax.bar(index + i * bar_width + j * bar_width * len(names),
                   library_data[col],
                   bar_width,
                   label=f"{library} - {col}",
                   color=colors(j),
                   alpha=alpha)
    
    title = f'Libraries execution time Comparsion ({size})'
    ax.set_xlabel('Operation')
    ax.set_ylabel(ylabel)
    ax.set_title(title)
    ax.set_xticks(index + bar_width * (len(names) - 1) / 2 + 0.3)
    ax.set_xticklabels(operations)
    ax.legend(title='Lib - Type')
    
    plt.tight_layout()
    plt.savefig(img_path + title.replace(' ', '_') + 
                    str(len(exclude_types) if exclude_types is not None else 0) +
                    str(len(exclude_operations) if exclude_operations is not None else 0))


if __name__ == '__main__':
    results: dict[dict[data_struct]] = {}
    for lib, path in res_pathes.items():
        results[lib] = dict()
        for size, test in res_tests.items():
            results[lib][size] = parse_results(lib, path + test)

    operations = ['add', 'mul', 'div', 'powui', 'sqrt']
    
    for lib, result in results.items():
        for size, test in result.items():
            test.plot_operations(exclude_operations=operations[3:])
            test.plot_operations(exclude_operations=operations[:3])
            test.plot_time_vs_bits()
            test.data.to_csv(tab_path + f'{lib}_{size}.csv', sep=';', index=True)
    

    plot_comparsion([
        results['GMP'][10000].data,
        results['NTL'][10000].data,
    ], ['GMP', 'NTL'], 10000, exclude_operations=operations[:3])
    plot_comparsion([
        results['GMP'][10000].data,
        results['NTL'][10000].data,
    ], ['GMP', 'NTL'], 10000, exclude_operations=operations[3:])