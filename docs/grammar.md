$$
\begin{align}
    [\text{Program}] &\to [\text{Statement}]^*
    \\
    [\text{Statement}] &\to 
    \begin{cases}
        \text{exit}([\text{Expression}]);
        \\
        \text{let}\space\text{identifier} = [\text{Expression}];
    \end{cases}
    \\
    [\text{Expression}] &\to
    \begin{cases}
        [\text{Term}]
        \\
        [\text{BinaryExpression}]
    \end{cases}
    \\
    [\text{Term}] &\to
    \begin{cases}
        \text{integer\_literal}
        \\
        \text{identifier}
    \end{cases}
    \\
    [\text{BinaryExpression}] &\to
    \begin{cases}
        [\text{Expression}] * \ [\text{Expression}] & \text{precedence} = 1
        \\
        [\text{Expression}] \ / \ \ [\text{Expression}] & \text{precedence} = 1
        \\
        [\text{Expression}] + [\text{Expression}] & \text{precedence} = 0
        \\
        [\text{Expression}] - [\text{Expression}] & \text{precedence} = 0
    \end{cases}
\end{align}
$$