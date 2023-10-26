/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.php                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zmakhkha <zmakhkha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:25:07 by zmakhkha          #+#    #+#             */
/*   Updated: 2023/10/26 18:25:08 by zmakhkha         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/


	<?php
	if ($_SERVER['REQUEST_METHOD'] === 'GET')
		echo "<h2>Merhbaaa bl GET asi ", $_GET["f_name"], " ", $_GET["f_name"], "</h2>";
	else if ($_SERVER['REQUEST_METHOD'] === 'POST')
		echo "<h2>Merhbaaa bl POST asi ", $_POSt["f_name"], " ", $_POST["f_name"], "</h2>";
	?>