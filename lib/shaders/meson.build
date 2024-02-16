python_module = import('python')

shaderc = find_program('glslangValidator')
python = python_module.find_installation('python3')
convert_to_header = files('convert_to_header.py')

# convert_to_header = find_program('convert_to_header')

comp_files = [
    'NormalDerivative0.comp',
    'NormalDerivative1.comp',
    'NormalDerivative2.comp',
    'MaxvellBoltzmannDerivative0.comp',
    'MaxvellBoltzmannDerivative1.comp',
    'RayleighDerivative0.comp',
    'RayleighDerivative1.comp',
]

generated_hpp_files = []

foreach comp_file : comp_files
    sprv_file = comp_file + '.spv'
    hpp_file = sprv_file + '.hpp'

    sprv_target = custom_target(
        comp_file + '_spv',
        input : comp_file,
        output : sprv_file,
        command : [shaderc, '@INPUT@', '-V', '-o', '@OUTPUT@']
    )

    hpp_target = custom_target(
        comp_file + '_hpp',
        input : sprv_target,
        output : hpp_file,
        command : [python, convert_to_header, '@INPUT@', '@OUTPUT@']
    )

    generated_hpp_files += hpp_target

endforeach
