import subprocess

testsize = [
    1000,
    10000,
    100000
]

for i in range(len(testsize)):
    size = str(testsize[i])
    sizek = str(testsize[i] // 1000)

    print("Running gmp " + sizek + 'k...')
    subprocess.run('gmp_test.exe -p gmp_result_' + sizek + 'k.txt -n ' + size)
    print("Running ntl " + sizek + 'k...')
    subprocess.run('ntl_test.exe -p ntl_result_' + sizek + 'k.txt -n ' + size)