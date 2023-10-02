$$
\begin{align}
    [\text{program}] &\to [\text{statement}]^*
    \\
    [\text{statement}] &\to 
    \begin{cases}
        \text{exit}([\text{expression}]);
        \\
        \text{let}\space\text{identifier} = [\text{expression}];
    \end{cases}
    \\
    [\text{expression}] &\to
    \begin{cases}
        \text{integer\_literal}
        \\
        \text{identifier}
    \end{cases}
\end{align}
$$