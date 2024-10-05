#include <stdio.h>

int main() {
    // Abre o arquivo HTML para escrita
    FILE *file = fopen("disco_rigido.html", "w");

    // Verifica se o arquivo foi aberto corretamente
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    // Escreve o conteúdo HTML no arquivo Cabeçalho .HTML
    fprintf(file, "<!DOCTYPE html>\n");
    fprintf(file, "<html lang=\"en\">\n");
    fprintf(file, "<head>\n");
    fprintf(file, "    <meta charset=\"UTF-8\">\n");
    fprintf(file, "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(file, "    <title>euBRASIL( Disco Binário )</title>\n");
    fprintf(file, "    <style>\n");
    fprintf(file, "        .circle-container { display: flex; justify-content: center; align-items: center; height: 70vh; }\n");
    fprintf(file, "        svg { width: 400px; height: 400px; }\n");
    fprintf(file, "        .meter-container { text-align: center; }\n");
    fprintf(file, "        .square { width: 1%%; height: 1%%; }\n");
    fprintf(file, "    </style>\n");
    fprintf(file, "</head>\n");
    fprintf(file, "<body>\n");
    fprintf(file, "    <div class=\"circle-container\">\n");
    fprintf(file, "        <div class=\"meter-container\">\n");

    // Loop para criar os elementos meter e labels
    for (char unidade = 'A'; unidade <= 'P'; unidade++) {
        fprintf(file, "            <label href=\"disk_%c\" for=\"disk_%c\">Unidade %c:</label>\n", unidade, unidade);
        fprintf(file, "            <meter id=\"disk_%c\" value=\"0.0\">0%%</meter><br>\n", unidade);
    }

    fprintf(file, "        </div>\n");
    fprintf(file, "        <div>\n");
    fprintf(file, "            <svg viewBox=\"-3 -3 45 45\">\n");

    // Loop para criar os círculos e caminhos SVG
    for (char unidade = 'A'; unidade <= 'P'; unidade++) {
        fprintf(file, "                <circle class=\"background\" cx=\"18\" cy=\"18\" r=\"%d\" fill=\"none\" stroke=\"#eef\" stroke-width=\"1\"/>\n", 21 - (unidade - 'A'));
        fprintf(file, "                <g id=\"disk_%c_path\"></g>\n", unidade);
    }

    fprintf(file, "            </svg>\n");
    fprintf(file, "        </div>\n");
    fprintf(file, "    </div>\n");
    fprintf(file, "    <img src=\"disco-hd-funcionando.gif\">\n");

    // JavaScript para animar os medidores
    fprintf(file, "    <script>\n");
    fprintf(file, "        document.addEventListener(\"DOMContentLoaded\", function() {\n");

    // Loop para criar os elementos JavaScript e funções de animação
    for (char unidade = 'A'; unidade <= 'P'; unidade++) {
        fprintf(file, "            const meter%c = document.getElementById('disk_%c');\n", unidade, unidade);
        fprintf(file, "            const path%c = document.getElementById('disk_%c_path');\n", unidade, unidade);
        fprintf(file, "            animateMeter(meter%c, path%c, %d, 0.%02d, 5000);\n", unidade, unidade, 21 - (unidade - 'A'), (unidade - 'A' + 1) * 5);
    }

    fprintf(file, "        });\n");

    // Função JavaScript de animação
    fprintf(file, "        function animateMeter(meter, path, radius, endValue, duration) {\n");
    fprintf(file, "            const increment = (endValue - 0) / (duration / 10);\n");
    fprintf(file, "            let currentValue = 0;\n");
    fprintf(file, "            function step() {\n");
    fprintf(file, "                currentValue += increment;\n");
    fprintf(file, "                if (currentValue <= endValue) {\n");
    fprintf(file, "                    meter.value = currentValue;\n");
    fprintf(file, "                    createSquares(path, currentValue, radius);\n");
    fprintf(file, "                    requestAnimationFrame(step);\n");
    fprintf(file, "                }\n");
    fprintf(file, "            }\n");
    fprintf(file, "            step();\n");
    fprintf(file, "        }\n");

    // Função JavaScript para criar os quadrados
    fprintf(file, "        function createSquares(path, value, radius) {\n");
    fprintf(file, "            const numSquares = Math.floor(value * 100);\n");
    fprintf(file, "            path.innerHTML = '';\n");
    fprintf(file, "            for (let i = 0; i < numSquares; i++) {\n");
    fprintf(file, "                const angle = (i / 100) * 2 * Math.PI - Math.PI / 2;\n");
    fprintf(file, "                const x = 18 + radius * Math.cos(angle);\n");
    fprintf(file, "                const y = 18 + radius * Math.sin(angle);\n");
    fprintf(file, "                const square = document.createElementNS(\"http://www.w3.org/2000/svg\", \"rect\");\n");
    fprintf(file, "                square.setAttribute('x', x);\n");
    fprintf(file, "                square.setAttribute('y', y);\n");
    fprintf(file, "                square.setAttribute('width', 0.6);\n");
    fprintf(file, "                square.setAttribute('height', 0.6);\n");
    fprintf(file, "                square.setAttribute('fill', (i %% 2 === 0) ? 'blue' : 'white');\n");
    fprintf(file, "                square.setAttribute('transform', `rotate(${angle * (180 / Math.PI)} ${x} ${y})`);\n");
    fprintf(file, "                path.appendChild(square);\n");
    fprintf(file, "            }\n");
    fprintf(file, "        }\n");

    fprintf(file, "    </script>\n");
    fprintf(file, "</body>\n");
    fprintf(file, "</html>\n");

    // Fecha o arquivo
    fclose(file);

    printf("O arquivo .HTML foi gerado com ***sucesso***\n");
    return 0;
}
