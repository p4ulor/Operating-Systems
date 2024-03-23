## Preparation
### i. 
Easy to follow. Done

### ii. 
0. In "/opt/isel/tvs/tvsapp/service":
1. Make file executable w/ `chmod 755 tvsapp@.service`
2. chmod 755 tvsapp@.service
3. sudo cp tvsapp@.service /etc/systemd/system
4. systemctl daemon-reload (optional, but must be used when updating the service, otherwise you get error: "Warning: The unit file, source configuration file or drop-ins of tvsapp@29901.service changed on disk. Run 'systemctl daemon-reload' to reload units.")

- sudo systemctl start tvsapp@"29901".service
- sudo systemctl start tvsapp@"29902".service
- sudo systemctl start tvsapp@"29903".service
- sudo systemctl start tvsapp@"29904".service

### iii.
- In "/opt/isel/tvs/nginx/sites-available":
- sudo cp tvsapp /etc/nginx/sites-available
- In "/etc/nginx/sites-enabled":
- sudo ln -s /etc/nginx/sites-available/tvsapp tvsapp (creates file link located in 1st param to the current path w/ name tvsapp)
- sudo systemctl restart nginx (pára e arranca outra vez. `Old alt -> sudo service nginx restart`)
- sudo systemctl reload nginx (faz sinal (Reloading is safer than restarting because if a syntax error is noticed in a config file, it will not proceed with the reload and your server remains running). `Old alt -> sudo service nginx reload`)

De facto observou-se que em http://localhost:12021/ o número à frente de "PORT:" na web-app ia mudando. E sempre incrementando, não tinha um padrão aleatório. E só se conseguiu testar este funcionamento numa janela anónima do chrome. Numa janela normal, o Port não variava por alguma razão inicialmente, mas depois ao reiniciar o chrome já funcionou. Durante os testes nunca "saiu" o porto 29904

## Recursos usados
- https://askubuntu.com/questions/1077778/how-do-i-pass-flags-when-starting-a-systemd-service
- https://ubuntu.com/tutorials/install-and-configure-nginx#5-activating-virtual-host-and-testing-results
